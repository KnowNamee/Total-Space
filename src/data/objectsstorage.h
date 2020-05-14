#ifndef OBJECTSSTORAGE_H
#define OBJECTSSTORAGE_H

#include <QString>
#include <map>
#include <set>
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
  static BuildingType GetBuildingType(const QString& caption);

  static UnitType GetUnitType(const QString& caption);
  static UnitRole GetUnitRole(const QString& role);
  static int32_t GetUnitPower(UnitType unit);
  static const UnitCharacteristics& GetUnitCharacteristics(UnitType unit);
  static const Resources& GetUnitCost(UnitType unit);
  static UnitRole GetUnitRole(UnitType unit);
  static UnitType GetUnitEnemy(UnitType unit);
  static const QString& GetUnitCaption(UnitType unit);

  static std::set<BuildingType> GetFirstLevelBuildings();
  static std::set<BuildingType> GetUpgrades(BuildingType building);
  static UnitType GetBuildingUnit(BuildingType building);

 private:
  static const std::map<QString, BuildingType> building_caption_to_type_;
  static const std::map<QString, UnitType> unit_caption_to_type_;
  static const std::map<QString, UnitRole> role_caption_to_role_;

  static std::map<BuildingType, const Building*> type_to_building_;
  static std::map<UnitType, const Unit*> type_to_unit_;
};

#endif  // OBJECTSSTORAGE_H
