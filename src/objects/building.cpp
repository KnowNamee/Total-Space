#include "objects/building.h"

Building::Building(const QString& caption, BuildingRole role,
                   const QVector<BuildingType>& upgrades, int32_t level,
                   int32_t time, UnitType unit, const Resources& cost,
                   const Resources& income)
    : caption_(caption),
      role_(role),
      upgrades_(upgrades),
      level_(level),
      time_(time),
      unit_(unit),
      cost_(cost),
      income_(income) {}

const Resources& Building::GetIncome() const { return income_; }

const QString& Building::GetCaption() const { return caption_; }

BuildingRole Building::GetBuildingRole() const { return role_; }

int32_t Building::GetLevel() const { return level_; }

int32_t Building::GetTime() const { return time_; }

const Resources& Building::GetCost() const { return cost_; }

const QVector<BuildingType>& Building::GetUpgrades() const { return upgrades_; }

UnitType Building::GetUnit() const { return unit_; }
