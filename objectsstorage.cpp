#include "objectsstorage.h"

#include <QString>

#include "building.h"

std::map<WarBuildingType, const WarBuilding *>
                                          ObjectsStorage::type_to_war_building_;
std::map<EconomicBuildingType, const EconomicBuilding *>
                                     ObjectsStorage::type_to_economic_building_;

void ObjectsStorage::AddBuilding(const EconomicBuilding *building_ptr) {
  if (building_ptr->GetCaption() == "Forge") {
    type_to_economic_building_[EconomicBuildingType::kForge] = building_ptr;
  }
  if (building_ptr->GetCaption() == "Workshop") {
    type_to_economic_building_[EconomicBuildingType::kWorkshop] = building_ptr;
  }
}

void ObjectsStorage::AddBuilding(const WarBuilding *building) {}
