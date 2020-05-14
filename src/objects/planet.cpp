#include "objects/planet.h"

#include <QDebug>
#include <QRandomGenerator>
#include <memory>

#include "core/planetsgraph.h"
#include "core/statemachine.h"
#include "data/loader.h"
#include "data/objectsstorage.h"
#include "graphics/planetgraphics.h"
#include "objects/player.h"
#include "scene/gamescene.h"

Planet::Planet(QPointF coordinates, double radius)
    : radius_(radius), coordinates_(coordinates) {}

void Planet::SetOwner(PlayerBase* owner) { owner_ = owner; }

const Resources& Planet::GetIncome() const { return income_; }

Resources Planet::GetUpgradeCost() const {
  return Resources((level_ + 1) * (level_ + 1) * 1000,
                   (level_ + 1) * (level_ + 1) * 1000);
}

void Planet::AddBuilding(BuildingType building) {
  buildings_.push_back(building);
  income_ += ObjectsStorage::GetIncome(building);
}

void Planet::AddUnit(UnitType unit) {
  units_on_planet_.push_back(unit);
  if (owner_ != nullptr) {
    owner_->IncreasePower(ObjectsStorage::GetUnitPower(unit));
  }
}

void Planet::AddUnits(const QVector<UnitType>& units) {
  tired_units_.append(units);
}

void Planet::RemoveUnit(UnitType unit) {
  units_on_planet_.removeOne(unit);
  if (owner_ != nullptr) {
    owner_->IncreasePower(-1 * ObjectsStorage::GetUnitPower(unit));
  }
}

void Planet::RemoveUnits(const QVector<UnitType>& units) {
  for (UnitType unit : units) {
    units_on_planet_.removeOne(unit);
  }
}

void Planet::Upgrade() {
  // TODO
  // Возможно стоит добавить что-то вроде увеличения дохода планеты
  // вопрос баланса
  level_++;
}

void Planet::Next() {
  units_on_planet_.append(tired_units_);
  tired_units_.clear();
}

int32_t Planet::GetToolsIncome() const { return income_.GetTools(); }
int32_t Planet::GetBatteriesIncome() const { return income_.GetBatteries(); }
QPointF Planet::GetCoordinates() const { return coordinates_; }

int32_t Planet::GetPower() const {
  int32_t power = 0;
  for (UnitType unit : units_on_planet_) {
    power += ObjectsStorage::GetUnitPower(unit);
  }
  for (UnitType unit : tired_units_) {
    power += ObjectsStorage::GetUnitPower(unit);
  }
  return power;
}
double Planet::GetRadius() const { return radius_; }

int32_t Planet::GetLevel() const { return level_; }
const QVector<BuildingType>& Planet::GetBuildings() const { return buildings_; }
const QVector<UnitType>& Planet::GetUnits() const { return units_on_planet_; }

const QVector<UnitType>& Planet::GetTiredUnits() const { return tired_units_; }

std::map<UnitType, UnitData> Planet::GetUnitsToData() const {
  std::map<UnitType, UnitData> units_to_data;
  QVector<UnitType> units = GetUnits();
  units.append(GetTiredUnits());
  bool is_reachable =
      Controller::scene->IsPlanetReachable(Controller::scene->GetPlayer());
  for (UnitType unit : units) {
    if (units_to_data[unit].quantity == 0) {
      if (is_reachable) {
        units_to_data[unit].unit_image = Loader::GetUnitImage(unit);
        units_to_data[unit].caption = ObjectsStorage::GetUnitCaption(unit);
      } else {
        // TODO
        // Картинка вопроса
        units_to_data[unit].unit_image = nullptr;
        units_to_data[unit].caption = "No Name";
      }
    }
    units_to_data[unit].quantity++;
  }
  return units_to_data;
}

PlanetGraphics* Planet::GetPlanetGraphics() const {
  return dynamic_cast<PlanetGraphics*>(
      Controller::scene->itemAt(2 * GetCoordinates(), QTransform()));
}

QVector<Planet*> Planet::GetNearestPlanets() const {
  return Controller::scene->GetGraph()->GetConnectedPlanets(
      GetPlanetGraphics());
}

PlayerBase* Planet::GetOwner() const { return owner_; }

std::set<BuildingType> Planet::GetAvailableBuildings() const {
  std::set<BuildingType> available_buildings;
  std::set<BuildingType> first_levels =
      ObjectsStorage::GetFirstLevelBuildings();
  available_buildings.insert(first_levels.begin(), first_levels.end());

  for (BuildingType building : buildings_) {
    std::set<BuildingType> upgrades = ObjectsStorage::GetUpgrades(building);
    available_buildings.insert(upgrades.begin(), upgrades.end());
  }
  return available_buildings;
}

std::set<UnitType> Planet::GetAvailableUnits() const {
  std::set<UnitType> available_units;
  for (BuildingType building : buildings_) {
    UnitType unit = ObjectsStorage::GetBuildingUnit(building);
    if (unit == UnitType::kNoUnit) {
      continue;
    }
    available_units.insert(unit);
  }
  return available_units;
}

bool Planet::IsBorder() const {
  for (Planet* planet : GetNearestPlanets()) {
    if (planet->GetOwner() != GetOwner()) {
      return false;
    }
  }
  return true;
}

std::map<Planet*, QVector<UnitType>> Planet::GetNonBorderUnits() const {

}

bool Planet::TakeAttack(
    const std::map<Planet*, QVector<UnitType>>& enemy_units) {
  AttackResult result = CalculateAttack(enemy_units);
  switch (result) {
    case AttackResult::kDraw: {
      return Draw(enemy_units, attack_points_);
    }
    case AttackResult::kLose: {
      return Lose(enemy_units);
    }
    default: {
      return Win(enemy_units, attack_points_);
    }
  }
}

Planet::AttackResult Planet::CalculateAttack(
    const std::map<Planet*, QVector<UnitType>>& enemy_units) {
  UnitCharacteristics enemy_characteristics;
  int32_t enemy_meele_count = 0;
  int32_t enemy_range_count = 0;
  // TODO
  // Коэффы рандомные, для баланса можно выбрать
  // Коэффициент показывающий во сколько увеличить силу армии,
  // если она сбалансированна
  const double kRoleBalancedCoefficient = 1.1;
  // Допустимая разница в сбалансированности
  const double kBalanceDifference = 0.2;
  // Если в отряде противника есть юниты, с которыми собственные юниты не любят
  // взаимодействовать, то уменьшаем собственную силу
  const double kEnemyUnitCoefficient = 0.7;
  // Дома и стены помогают, коэфф увеличивающий характеристики защитников
  const double kHomeCoefficient = 1.1;

  std::set<UnitType> enemy_units_types;
  for (const auto& planet_to_unit : enemy_units) {
    for (UnitType unit : planet_to_unit.second) {
      enemy_units_types.insert(unit);
      if (ObjectsStorage::GetUnitRole(unit) == UnitRole::kMelee) {
        enemy_meele_count++;
      } else {
        enemy_range_count++;
      }

      if (units_on_planet_.contains(ObjectsStorage::GetUnitEnemy(unit))) {
        enemy_characteristics += ObjectsStorage::GetUnitCharacteristics(unit) *
                                 kEnemyUnitCoefficient;
        continue;
      }
      enemy_characteristics += ObjectsStorage::GetUnitCharacteristics(unit);
    }
  }

  int32_t self_meele_count = 0;
  int32_t self_range_count = 0;
  int32_t self_power = 0;
  UnitCharacteristics self_characteristics;
  for (UnitType unit : units_on_planet_) {
    self_power += ObjectsStorage::GetUnitPower(unit);
    if (ObjectsStorage::GetUnitRole(unit) == UnitRole::kMelee) {
      self_meele_count++;
    } else {
      self_range_count++;
    }

    if (enemy_units_types.find(ObjectsStorage::GetUnitEnemy(unit)) !=
        enemy_units_types.end()) {
      self_characteristics +=
          ObjectsStorage::GetUnitCharacteristics(unit) * kEnemyUnitCoefficient;
      continue;
    }
    self_characteristics += ObjectsStorage::GetUnitCharacteristics(unit);
  }

  double enemy_army_balance = 1;
  if (enemy_range_count != 0) {
    enemy_army_balance = std::abs(1 - static_cast<double>(enemy_meele_count) /
                                          enemy_range_count);
  }
  double self_army_balance = 1;
  if (self_range_count != 0) {
    self_army_balance =
        std::abs(1 - static_cast<double>(self_meele_count) / self_range_count);
  }

  if (enemy_army_balance <= kBalanceDifference) {
    enemy_characteristics *= kRoleBalancedCoefficient;
  }

  if (self_army_balance <= kBalanceDifference) {
    self_characteristics *= kRoleBalancedCoefficient;
  }

  self_characteristics *= kHomeCoefficient;

  std::pair<int32_t, int32_t> points =
      CountPoints(self_characteristics, enemy_characteristics);
  attack_points_ = points;
  int32_t self_points = points.first;
  int32_t enemy_points = points.second;

  const int32_t kDrawDifference = 2;
  if (abs(self_points - enemy_points) < kDrawDifference) {
    return AttackResult::kDraw;
  }
  if (self_points > enemy_points) {
    return AttackResult::kLose;
  }
  return AttackResult::kWin;
}

std::pair<int32_t, int32_t> Planet::CountPoints(
    const UnitCharacteristics& self_characteristics,
    const UnitCharacteristics& enemy_characteristics) {
  int32_t self_points = 0;
  int32_t enemy_points = 0;

  // TODO
  // Вклад каждой характеристики можно выбрать
  const int32_t kAttackPoints = 4;
  const int32_t kArmorPoints = 3;
  const int32_t kHealthPoints = 4;
  const int32_t kStaminaPoints = 5;

  if (self_characteristics.GetAttack() > enemy_characteristics.GetAttack()) {
    self_points += kAttackPoints;
    if (self_characteristics.GetAttack() - enemy_characteristics.GetAttack() <
        self_characteristics.GetAttack() / 3) {
      enemy_points += kAttackPoints / 2;
    }
  } else if (self_characteristics.GetAttack() <
             enemy_characteristics.GetAttack()) {
    enemy_points += kAttackPoints;
    if (enemy_characteristics.GetAttack() - self_characteristics.GetAttack() <
        enemy_characteristics.GetAttack() / 3) {
      self_points += kAttackPoints / 2;
    }
  }

  if (self_characteristics.GetArmor() > enemy_characteristics.GetArmor()) {
    self_points += kArmorPoints;
    if (self_characteristics.GetArmor() - enemy_characteristics.GetArmor() <
        self_characteristics.GetArmor() / 3) {
      enemy_points += kArmorPoints / 2;
    }
  } else if (self_characteristics.GetArmor() <
             enemy_characteristics.GetArmor()) {
    enemy_points += kArmorPoints;
    if (enemy_characteristics.GetArmor() - self_characteristics.GetArmor() <
        self_characteristics.GetArmor() / 2) {
      self_points += kArmorPoints / 2;
    }
  }

  if (self_characteristics.GetHealth() > enemy_characteristics.GetHealth()) {
    self_points += kHealthPoints;
    if (self_characteristics.GetHealth() - enemy_characteristics.GetHealth() <
        self_characteristics.GetHealth() / 2) {
      enemy_points += kHealthPoints / 2;
    }
  } else if (self_characteristics.GetHealth() <
             enemy_characteristics.GetHealth()) {
    enemy_points += kHealthPoints;
    if (enemy_characteristics.GetHealth() - self_characteristics.GetHealth() <
        self_characteristics.GetHealth() / 2) {
      self_points += kHealthPoints / 2;
    }
  }

  if (self_characteristics.GetStamina() > enemy_characteristics.GetStamina()) {
    self_points += kStaminaPoints;
    if (self_characteristics.GetStamina() - enemy_characteristics.GetStamina() <
        self_characteristics.GetStamina() / 2) {
      enemy_points += kStaminaPoints / 2;
    }
  } else if (self_characteristics.GetStamina() <
             enemy_characteristics.GetStamina()) {
    enemy_points += kStaminaPoints;
    if (enemy_characteristics.GetStamina() - self_characteristics.GetStamina() <
        self_characteristics.GetStamina() / 2) {
      self_points += kStaminaPoints / 2;
    }
  }

  return std::make_pair(self_points, enemy_points);
}

bool Planet::Lose(const std::map<Planet*, QVector<UnitType>>& enemy_units) {
  double random_double = QRandomGenerator::global()->generateDouble();
  const double kDeadCoefficient = std::min(random_double, 1. - random_double);
  std::map<Planet*, QVector<UnitType>> enemy_units_copy = enemy_units;
  for (auto& planet_to_units : enemy_units_copy) {
    int32_t number_of_dead_units =
        std::min(static_cast<int32_t>(std::ceil(kDeadCoefficient *
                                                planet_to_units.second.size())),
                 planet_to_units.second.size());

    for (int32_t i = 0; i < number_of_dead_units; i++) {
      int32_t index =
          (static_cast<int32_t>(QRandomGenerator::global()->generate()) %
               planet_to_units.second.size() +
           planet_to_units.second.size()) %
          planet_to_units.second.size();
      planet_to_units.first->RemoveUnit(planet_to_units.second[index]);
      planet_to_units.second.remove(index);
    }
  }

  for (const auto& planet_to_units : enemy_units_copy) {
    planet_to_units.first->RemoveUnits(planet_to_units.second);
    planet_to_units.first->AddUnits(planet_to_units.second);
  }
  return false;
}

bool Planet::Draw(const std::map<Planet*, QVector<UnitType>>& enemy_units,
                  const std::pair<int32_t, int32_t>& points) {
  bool probability = QRandomGenerator::global()->generate() % 2;
  if (probability) {
    return Lose(enemy_units);
  } else {
    return Win(enemy_units, points);
  }
}

bool Planet::Win(const std::map<Planet*, QVector<UnitType>>& enemy_units,
                 const std::pair<int32_t, int32_t>& points) {
  if (enemy_units.size() == 0) {
    return false;
  }
  std::map<Planet*, QVector<UnitType>> enemy_units_copy = enemy_units;

  const double kMaxDeadCoefficient = 0.5;
  const double kDeadCoefficient =
      kMaxDeadCoefficient * static_cast<double>(points.first) / points.second;
  for (auto& planet_to_units : enemy_units_copy) {
    int32_t number_of_dead_units = std::min(
        static_cast<int32_t>(
            std::floor(kDeadCoefficient * planet_to_units.second.size())),
        planet_to_units.second.size());

    for (int32_t i = 0; i < number_of_dead_units; i++) {
      int32_t index =
          (static_cast<int32_t>(QRandomGenerator::global()->generate()) %
               planet_to_units.second.size() +
           planet_to_units.second.size()) %
          planet_to_units.second.size();
      planet_to_units.first->RemoveUnit(planet_to_units.second[index]);
      planet_to_units.second.erase(planet_to_units.second.begin() + index);
    }
  }

  PlayerBase* enemy = enemy_units.begin()->first->GetOwner();
  for (UnitType unit : units_on_planet_) {
    RemoveUnit(unit);
  }
  MoveUnits(enemy_units_copy);
  if (owner_ != nullptr) {
    owner_->RemovePlanet(this);
  }
  SetOwner(enemy);
  enemy->AddPlanet(this);
  return true;
}

void Planet::MoveUnits(
    const std::map<Planet*, QVector<UnitType>>& enemy_units) {
  for (const auto& planet_to_unit : enemy_units) {
    planet_to_unit.first->RemoveUnits(planet_to_unit.second);
    AddUnits(planet_to_unit.second);
  }
}
