#include "data/objectsstorage.h"

#include <QDebug>
#include <QString>

#include "objects/building.h"
#include "objects/unit.h"

const std::map<QString, BuildingType>
    ObjectsStorage::building_caption_to_type_ = {
        std::make_pair("Forge", BuildingType::kForge),
        std::make_pair("Workshop", BuildingType::kWorkshop),
        std::make_pair("Battery Factory", BuildingType::kBatteryFactory),
        std::make_pair("Assembly Shop", BuildingType::kAssemblyShop),
        std::make_pair("Electronics", BuildingType::kElectronics),
        std::make_pair("Robots Line", BuildingType::kRobotsLine),
        std::make_pair("Barracks", BuildingType::kBarracks),
        std::make_pair("Spaceport", BuildingType::kSpaceport),
        std::make_pair("Training Base", BuildingType::kTrainingBase)};

const std::map<QString, UnitType> ObjectsStorage::unit_caption_to_type_ = {
    std::make_pair("No Unit", UnitType::kNoUnit),
    std::make_pair("Rover", UnitType::kRover),
    std::make_pair("Falcon", UnitType::kFalcon),
    std::make_pair("Marine", UnitType::kMarine),
    std::make_pair("Ranger", UnitType::kRanger),
    std::make_pair("Droid", UnitType::kDroid)};

const std::map<QString, UnitRole> ObjectsStorage::role_caption_to_role_ = {
    std::make_pair("Melee", UnitRole::kMelee),
    std::make_pair("Range", UnitRole::kRange),
    std::make_pair("Healer", UnitRole::kHealer)};

const std::map<QString, BuildingRole>
    ObjectsStorage::role_caption_to_building_role_ = {
        std::make_pair("batteries", BuildingRole::kBatteries),
        std::make_pair("tools", BuildingRole::kTools),
        std::make_pair("war", BuildingRole::kWar)};

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

const Resources& ObjectsStorage::GetBuildingCost(BuildingType building) {
  return type_to_building_.at(building)->GetCost();
}

QVector<UnitType> ObjectsStorage::GetAllPossibleUnits() {
  QVector<UnitType> units;
  for (const auto& caption_to_type : unit_caption_to_type_) {
    if (caption_to_type.second != UnitType::kNoUnit) {
      units.push_back(caption_to_type.second);
    }
  }
  return units;
}

UnitRole ObjectsStorage::GetUnitRole(const QString& role) {
  return role_caption_to_role_.at(role);
}

BuildingRole ObjectsStorage::GetBuildingRole(const QString& role) {
  return role_caption_to_building_role_.at(role);
}

int32_t ObjectsStorage::GetUnitPower(UnitType unit) {
  return type_to_unit_[unit]->GetPower();
}

const UnitCharacteristics& ObjectsStorage::GetUnitCharacteristics(
    UnitType unit) {
  return type_to_unit_[unit]->GetUnitCharacteristics();
}

const Resources& ObjectsStorage::GetUnitCost(UnitType unit) {
  return type_to_unit_[unit]->GetCost();
}

UnitRole ObjectsStorage::GetUnitRole(UnitType unit) {
  return type_to_unit_[unit]->GetUnitRole();
}

BuildingRole ObjectsStorage::GetBuildingRole(BuildingType building) {
  return type_to_building_[building]->GetBuildingRole();
}

UnitType ObjectsStorage::GetUnitEnemy(UnitType unit) {
  return type_to_unit_[unit]->GetUnitEnemy();
}

const QString& ObjectsStorage::GetUnitCaption(UnitType unit) {
  return type_to_unit_.at(unit)->GetCaption();
}

std::set<BuildingType> ObjectsStorage::GetFirstLevelBuildings() {
  std::set<BuildingType> first_level_buildings;
  for (const auto& building_pair : type_to_building_) {
    if (building_pair.second->GetLevel() == 1) {
      first_level_buildings.insert(building_pair.first);
    }
  }
  return first_level_buildings;
}

std::set<BuildingType> ObjectsStorage::GetUpgrades(BuildingType building) {
  std::set<BuildingType> upgrades;
  for (BuildingType upgrade : type_to_building_.at(building)->GetUpgrades()) {
    upgrades.insert(upgrade);
  }
  return upgrades;
}

UnitType ObjectsStorage::GetBuildingUnit(BuildingType building) {
  return type_to_building_.at(building)->GetUnit();
}

const Unit* ObjectsStorage::GetUnit(UnitType unit) {
  return type_to_unit_[unit];
}

const Building* ObjectsStorage::GetBuilding(BuildingType building) {
  return type_to_building_[building];
}

const QString& ObjectsStorage::GetBuildingCaption(BuildingType building) {
  return type_to_building_.at(building)->GetCaption();
}
