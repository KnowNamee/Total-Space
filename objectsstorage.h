#ifndef OBJECTSSTORAGE_H
#define OBJECTSSTORAGE_H

#include <map>
#include <memory>

#include "utility.h"

class EconomicBuilding;
class WarBuilding;

class ObjectsStorage {
 public:
  ObjectsStorage() = delete;

  static void AddBuilding(const EconomicBuilding* building);
  static void AddBuilding(const WarBuilding* building);

 private:
  static std::map<EconomicBuildingType, const EconomicBuilding*>
                                                     type_to_economic_building_;
  static std::map<WarBuildingType, const WarBuilding*> type_to_war_building_;
};

#endif  // OBJECTSSTORAGE_H
