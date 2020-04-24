#include "data/objectsstorage.h"

#include <QString>
#include <QSet>

#include "objects/building.h"
#include "objects/unit.h"

const std::map<QString, BuildingType>
    ObjectsStorage::building_caption_to_type_ = {
        std::make_pair("Forge", BuildingType::kForge),
        std::make_pair("Workshop", BuildingType::kWorkshop),
        std::make_pair("Battery Factory", BuildingType::kBatteryFactory),
        std::make_pair("Assembly Shop", BuildingType::kAssemblyShop),
        std::make_pair("Electronics", BuildingType::kElectronics),
        std::make_pair("Robots Line", BuildingType::kRobotsLine)};

const std::map<QString, UnitType> ObjectsStorage::unit_caption_to_type_ = {
    std::make_pair("No Unit", UnitType::kNoUnit),
    std::make_pair("Rover", UnitType::kRover),
    std::make_pair("Falcon", UnitType::kFalcon),
    std::make_pair("Marine", UnitType::kMarine),
    std::make_pair("Ranger", UnitType::kRanger),
    std::make_pair("Droid", UnitType::kDroid)};

std::map<BuildingType, const Building*> ObjectsStorage::type_to_building_;
std::map<UnitType, const Unit*> ObjectsStorage::type_to_unit_;

void ObjectsStorage::AddBuilding(const Building* building_ptr) {
  type_to_building_[ObjectsStorage::building_caption_to_type_.at(
      building_ptr->GetCaption())] = building_ptr;
}

void ObjectsStorage::AddUnit(const Unit* unit) {
  type_to_unit_[ObjectsStorage::unit_caption_to_type_.at(unit->GetCaption())] =
      unit;
}

const Resources& ObjectsStorage::GetIncome(BuildingType building) {
  return type_to_building_.at(building)->GetIncome();
}

UnitType ObjectsStorage::GetUnitType(const QString& caption) {
  return unit_caption_to_type_.at(caption);
}

BuildingType ObjectsStorage::GetBuildingType(const QString& caption) {
  return building_caption_to_type_.at(caption);
}

std::set<BuildingType> ObjectsStorage::GetFirstLevelBuildings() {
  std::set<BuildingType> first_level_buildings;
  for (auto building_pair : type_to_building_) {
    if (building_pair.second->GetLevel() == 1)  {
      first_level_buildings.insert(building_pair.first);
    }
  }
  return first_level_buildings;
}

std::set<BuildingType> ObjectsStorage::GetUpgrades(BuildingType building) {
  std::set<BuildingType> upgrades;
  for(BuildingType upgrade : type_to_building_.at(building)->GetUpgrades()) {
    upgrades.insert(upgrade);
  }
  return upgrades;
}

UnitType ObjectsStorage::GetBuildingUnit(BuildingType building) {
  return type_to_building_.at(building)->GetUnit();
}
