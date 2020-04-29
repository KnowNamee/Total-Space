#include "objects/planet.h"

#include "data/objectsstorage.h"

Planet::Planet(QPointF coordinates, double radius)
    : radius_(radius), coordinates_(coordinates) {}

void Planet::SetOwner(const std::shared_ptr<PlayerBase>& owner) {
  owner_ = owner;
}

const Resources& Planet::GetIncome() const { return income_; }

void Planet::AddBuilding(BuildingType building) {
  buildings_.push_back(building);
  income_ += ObjectsStorage::GetIncome(building);
}

void Planet::AddUnit(UnitType unit) { units_on_planet_.push_back(unit); }

void Planet::Upgrade() {
  // TODO
  // Возможно стоит добавить что-то вроде увеличения дохода планеты
  // вопрос баланса
  level_++;
}

int32_t Planet::GetToolsIncome() const { return income_.GetTools(); }
int32_t Planet::GetBatteriesIncome() const { return income_.GetBatteries(); }
QPointF Planet::GetCoordinates() const { return coordinates_; }
double Planet::GetRadius() const { return radius_; }
const QVector<BuildingType>& Planet::GetBuildings() const { return buildings_; }
const QVector<UnitType>& Planet::GetUnits() const { return units_on_planet_; }

PlayerBase* Planet::GetOwner() const { return owner_.get(); }

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
