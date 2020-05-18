#include "bot.h"

#include <QDebug>
#include <QRandomGenerator>
#include <cmath>

#include "core/statemachine.h"
#include "data/objectsstorage.h"
#include "scene/gamescene.h"
#include "scene/gameview.h"

Bot::Bot(Planet* planet, const QString& color)
    : PlayerBase(planet, PlayerBase::Type::kBot, color) {
  for (UnitType unit : planet->GetUnits()) {
    IncreasePower(ObjectsStorage::GetUnitPower(unit));
  }
}

Bot::~Bot() {}

void Bot::Next() {
  ApplyAttackStrategy();
  ApplyEconomicStrategy();
  UpdateResources();
}

const QVector<std::pair<Planet*, Planet*>>& Bot::GetPlanetsToShow() const {
  return planets_to_show_;
}

void Bot::ClearPlanetToShow() { planets_to_show_.clear(); }

void Bot::ApplyAttackStrategy() {
  Resources attack_resources = GetResources() * kAttackResources;
  for (Planet* planet : GetPlanets()) {
    if (!Controller::scene->IsPlanetOnScene(planet)) {
      continue;
    }
    if (!planet->IsBorder() || planet->GetOwner() != this) {
      continue;
    }
    QVector<UnitType> units_on_planet = planet->GetUnits();
    units_on_planet.append(planet->GetTiredUnits());
    // Пробуем свои силы
    if (planet->IsAbleToDefend(units_on_planet)) {
      TryAttack(planet, &attack_resources);
      continue;
    }
    // Достаем кошелек
    std::function<bool(Planet*, QVector<UnitType>, Planet*)> predicate =
        &Planet::IsAbleToDefend;
    Resources necessary_resources = BinarySearchResources(
        predicate, units_on_planet, planet, attack_resources);
    QVector<UnitType> units_to_buy =
        planet->GetMostProfitableUnits(units_on_planet, necessary_resources);
    units_on_planet.append(units_to_buy);
    if (planet->IsAbleToDefend(units_on_planet)) {
      Resources before_purchase = GetResources();
      planet->BuyUnits(units_to_buy);
      attack_resources -= (before_purchase - GetResources());
      continue;
    }
    // Подтягиваем корешей
    std::map<Planet*, QVector<UnitType>> nearest_units =
        planet->GetNearestNonBorderUnits();
    std::map<Planet*, QVector<UnitType>> units_to_move;
    for (const auto& planet_to_units : nearest_units) {
      if (planet_to_units.second.empty()) {
        continue;
      }
      units_on_planet.append(planet_to_units.second);
      units_to_move[planet_to_units.first] = planet_to_units.second;
      if (planet->IsAbleToDefend(units_on_planet)) {
        Resources before_purchase = GetResources();
        planet->BuyUnits(units_to_buy);
        attack_resources -= (before_purchase - GetResources());
        planet->MoveUnits(units_to_move);
        continue;
      }
    }

    // Тікаем з гораду
    QVector<UnitType> real_units = planet->GetUnits();
    std::set<Planet*> nearest_nonborder_planets;
    std::set<Planet*> nearest_border_planets;
    for (Planet* nearest_planet : planet->GetNearestPlanets()) {
      if (nearest_planet->IsBorder() && nearest_planet->GetOwner() == this) {
        nearest_border_planets.insert(nearest_planet);
        continue;
      }
      if (nearest_planet->GetOwner() == this) {
        nearest_nonborder_planets.insert(nearest_planet);
      }
    }
    if (!nearest_nonborder_planets.empty()) {
      RunFromPlanet(planet, nearest_nonborder_planets);
      continue;
    }
    if (!nearest_border_planets.empty()) {
      RunFromPlanet(planet, nearest_border_planets);
      continue;
    }
  }
}

void Bot::ApplyEconomicStrategy() {
  Resources available_resources = GetResources();
  for (Planet* planet : GetPlanets()) {
    if (!Controller::scene->IsPlanetOnScene(planet)) {
      continue;
    }
    if (!planet->IsSafe()) {
      continue;
    }
    TryBuild(planet, &available_resources, BuildingRole::kEconomic);
  }
}

void Bot::TryAttack(Planet* planet, Resources* available_resources) {
  std::map<PlayerBase*, QVector<Planet*>> enemies = planet->GetNearestEnemies();
  PlayerBase* max_player = enemies.begin()->first;
  for (const auto& player_to_planets : enemies) {
    if (player_to_planets.second.size() > enemies[max_player].size()) {
      max_player = player_to_planets.first;
    }
  }
  Planet* planet_to_attack = nullptr;
  if (!enemies[max_player].empty()) {
    planet_to_attack = enemies[max_player][static_cast<int32_t>(
        QRandomGenerator::global()->generate() %
        static_cast<uint32_t>(enemies[max_player].size()))];
  } else {
    for (Planet* planet_for_attack : planet->GetNearestPlanets()) {
      if (planet_for_attack->GetOwner() == nullptr) {
        planet_to_attack = planet_for_attack;
        break;
      }
    }
  }
  QVector<UnitType> units_on_planet = planet->GetUnits();
  QVector<UnitType> units_to_attack;
  if (planet_to_attack == nullptr) {
    return;
  }
  for (int32_t i = 0; i < units_on_planet.size(); i++) {
    units_to_attack.push_back(units_on_planet[i]);
    if (planet_to_attack->TryTakeAttack(units_to_attack, planet)) {
      QVector<UnitType> defending_units = units_on_planet;
      for (UnitType unit_to_attack : units_to_attack) {
        defending_units.removeOne(unit_to_attack);
      }
      defending_units.append(planet->GetTiredUnits());

      std::function<bool(Planet*, QVector<UnitType>, Planet*)> predicate =
          &Planet::IsAbleToDefend;
      Resources necessary_resources =
          BinarySearchResources(predicate, defending_units, planet,
                                *available_resources, planet_to_attack);
      QVector<UnitType> units_to_buy =
          planet->GetMostProfitableUnits(defending_units, necessary_resources);
      defending_units.append(units_to_buy);

      if (planet->IsAbleToDefend(defending_units, planet_to_attack)) {
        std::map<Planet*, QVector<UnitType>> attacking_units = {
            std::make_pair(planet, units_to_attack)};
        planet_to_attack->TakeAttack(attacking_units);
        planets_to_show_.push_back(std::make_pair(planet, planet_to_attack));

        planet->BuyUnits(units_to_buy);
        *available_resources -= necessary_resources;
        return;
      }
      break;
    }
  }
  std::function<bool(Planet*, QVector<UnitType>, Planet*)> predicate =
      &Planet::TryTakeAttack;
  Resources necessary_resources =
      BinarySearchResources(predicate, units_on_planet, planet_to_attack,
                            *available_resources, planet);
  QVector<UnitType> units_to_buy =
      planet->GetMostProfitableUnits(units_to_attack, necessary_resources);
  units_to_attack.append(units_to_buy);
  if (planet_to_attack->TryTakeAttack(units_to_attack, planet)) {
    QVector<UnitType> new_units = planet->GetTiredUnits();
    std::function<bool(Planet*, QVector<UnitType>, Planet*)> predicate =
        &Planet::IsAbleToDefend;
    Resources defence_resources = BinarySearchResources(
        predicate, new_units, planet,
        *available_resources - necessary_resources, planet_to_attack);
    QVector<UnitType> units_to_defence =
        planet->GetMostProfitableUnits(new_units, defence_resources);
    new_units.append(units_to_defence);
    if (planet->IsAbleToDefend(new_units, planet_to_attack)) {
      planet->BuyUnits(units_to_buy);
      *available_resources -= necessary_resources;
      std::map<Planet*, QVector<UnitType>> attack;
      attack[planet] = planet->GetUnits();
      planet_to_attack->TakeAttack(attack);
      planets_to_show_.push_back(std::make_pair(planet, planet_to_attack));

      planet->BuyUnits(units_to_defence);
      *available_resources -= defence_resources;
      return;
    }

    std::map<Planet*, QVector<UnitType>> nearest_units =
        planet->GetNearestNonBorderUnits();
    std::map<Planet*, QVector<UnitType>> units_to_move;
    for (const auto& planet_to_units : nearest_units) {
      if (planet_to_units.second.empty()) {
        continue;
      }
      new_units.append(planet_to_units.second);
      units_to_move[planet_to_units.first] = planet_to_units.second;
      if (planet->IsAbleToDefend(new_units, planet_to_attack)) {
        planet->BuyUnits(units_to_buy);
        *available_resources -= necessary_resources;
        std::map<Planet*, QVector<UnitType>> attack;
        attack[planet] = planet->GetUnits();
        planet_to_attack->TakeAttack(attack);
        planets_to_show_.push_back(std::make_pair(planet, planet_to_attack));

        planet->BuyUnits(units_to_defence);
        *available_resources -= defence_resources;
        planet->MoveUnits(units_to_move);
        return;
      }
    }
  }
  TryBuild(planet, available_resources, BuildingRole::kWar);
}

void Bot::TryBuild(Planet* planet, Resources* available_resources,
                   BuildingRole role) {
  BuildingType building_to_buy = planet->GetMostProfitableBuilding(
      *available_resources, kUpgradeCoefficient, role);
  if (building_to_buy == BuildingType::kUpgrade) {
    Resources before = GetResources();
    planet->Upgrade();
    *available_resources -= (before - GetResources());
    return;
  }
  if (building_to_buy != BuildingType::kNoBuilding) {
    planet->BuyBuildinng(building_to_buy);
    *available_resources -= ObjectsStorage::GetBuildingCost(building_to_buy);
  }
}

void Bot::RunFromPlanet(Planet* planet, std::set<Planet*> planets_to_run) {
  QVector<UnitType> real_units = planet->GetUnits();
  int32_t number_to_move = static_cast<int32_t>(std::ceil(
      static_cast<double>(real_units.size()) / planets_to_run.size()));
  for (Planet* nearest_planet : planets_to_run) {
    std::map<Planet*, QVector<UnitType>> units_to_move;
    for (int32_t i = 0; i < std::min(number_to_move, real_units.size()); i++) {
      units_to_move[planet].push_back(real_units[i]);
    }
    nearest_planet->MoveUnits(units_to_move);
    real_units = planet->GetUnits();
  }
}

Resources Bot::BinarySearchResources(
    std::function<bool(Planet*, QVector<UnitType>, Planet*)> predicate,
    const QVector<UnitType>& units, Planet* planet, const Resources& resources,
    Planet* aim) {
  Resources left(0, 0);
  Resources right = resources;

  while (left < right) {
    Resources middle = (left + right) / 2;
    QVector<UnitType> possible_units = units;
    possible_units.append(planet->GetMostProfitableUnits(units, middle));

    if (predicate(planet, possible_units, aim)) {
      right = middle;
    } else {
      left = middle + Resources(1, 1);
    }
  }
  return left;
}
