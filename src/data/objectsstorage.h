#ifndef OBJECTSSTORAGE_H
#define OBJECTSSTORAGE_H

#include <QString>
#include <map>
#include <memory>
#include <set>

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
  static const Resources& GetBuildingCost(BuildingType building);
  static int32_t GetBuildingLevel(BuildingType building);

  static QVector<UnitType> GetAllPossibleUnits();
  static UnitType GetUnitType(const QString& caption);
  static UnitRole GetUnitRole(const QString& role);
  static BuildingRole GetBuildingRole(const QString& role);
  static int32_t GetUnitPower(UnitType unit);
  static const UnitCharacteristics& GetUnitCharacteristics(UnitType unit);
  static const Resources& GetUnitCost(UnitType unit);
  static UnitRole GetUnitRole(UnitType unit);
  static BuildingRole GetBuildingRole(BuildingType building);
  static UnitType GetUnitEnemy(UnitType unit);
  static const QString& GetUnitCaption(UnitType unit);

  static std::set<BuildingType> GetFirstLevelBuildings();
  static std::set<BuildingType> GetUpgrades(BuildingType building);
  static UnitType GetBuildingUnit(BuildingType building);
  static int32_t GetBuildingTime(BuildingType building);

  static const Unit* GetUnit(UnitType unit);
  static const Building* GetBuilding(BuildingType building);

  static const QString& GetBuildingCaption(BuildingType building);

 private:
  static const std::map<QString, BuildingType> building_caption_to_type_;
  static const std::map<QString, UnitType> unit_caption_to_type_;
  static const std::map<QString, UnitRole> role_caption_to_role_;
  static const std::map<QString, BuildingRole> role_caption_to_building_role_;

  static std::map<BuildingType, const Building*> type_to_building_;
  static std::map<UnitType, const Unit*> type_to_unit_;
};

#endif  // OBJECTSSTORAGE_H
