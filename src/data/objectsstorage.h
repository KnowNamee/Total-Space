#ifndef OBJECTSSTORAGE_H
#define OBJECTSSTORAGE_H

#include <QString>
#include <map>
#include <memory>

#include "util/utility.h"

class Building;
class Unit;

class ObjectsStorage {
 public:
  ObjectsStorage() = delete;

  static void AddBuilding(const Building* building);
  static void AddUnit(const Unit* unit);
  static const Resources& GetIncome(BuildingType building);
  static UnitType GetUnitType(const QString& caption);
  static BuildingType GetBuildingType(const QString& caption);

 private:
  static const std::map<QString, BuildingType> building_caption_to_type_;
  static const std::map<QString, UnitType> unit_caption_to_type_;

  static std::map<BuildingType, const Building*> type_to_building_;
  static std::map<UnitType, const Unit*> type_to_unit_;
};

#endif  // OBJECTSSTORAGE_H
