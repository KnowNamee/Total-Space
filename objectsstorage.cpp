#include "objectsstorage.h"

#include <QString>

#include "building.h"
#include "unit.h"

const std::map<QString, EconomicBuildingType>
    ObjectsStorage::economic_caption_to_type_ = {
        std::make_pair("Forge", EconomicBuildingType::kForge),
        std::make_pair("Workshop", EconomicBuildingType::kWorkshop),
        std::make_pair("BatterFactory", EconomicBuildingType::kBatteryFactory)};

const std::map<QString, WarBuildingType> ObjectsStorage::war_caption_to_type_;

const std::map<QString, UnitType> ObjectsStorage::unit_caption_to_type_ = {
    std::make_pair("Rover", UnitType::kRover),
    std::make_pair("Falcon", UnitType::kFalcon),
    std::make_pair("Marine", UnitType::kMarine),
    std::make_pair("Ranger", UnitType::kRanger)};

std::map<WarBuildingType, const WarBuilding *>
    ObjectsStorage::type_to_war_building_;
std::map<EconomicBuildingType, const EconomicBuilding *>
    ObjectsStorage::type_to_economic_building_;
std::map<UnitType, const Unit *> ObjectsStorage::type_to_unit_;

void ObjectsStorage::AddBuilding(const EconomicBuilding *building_ptr) {
  type_to_economic_building_[ObjectsStorage::economic_caption_to_type_.at(
      building_ptr->GetCaption())] = building_ptr;
}

void ObjectsStorage::AddBuilding(const WarBuilding *building_ptr) {}

void ObjectsStorage::AddUnit(const Unit *unit) {
  type_to_unit_[ObjectsStorage::unit_caption_to_type_.at(unit->GetCaption())] =
      unit;
}
