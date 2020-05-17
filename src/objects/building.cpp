#include "objects/building.h"

Building::Building(const QString& caption, const QString& type,
                   const QVector<BuildingType>& upgrades, int32_t level,
                   UnitType unit, const Resources& cost,
                   const Resources& income)
    : caption_(caption),
      type_(type),
      upgrades_(upgrades),
      level_(level),
      unit_(unit),
      cost_(cost),
      income_(income) {}

const Resources& Building::GetIncome() const { return income_; }

const QString& Building::GetCaption() const { return caption_; }

int32_t Building::GetLevel() const { return level_; }

const Resources& Building::GetCost() const { return cost_; }

const QVector<BuildingType>& Building::GetUpgrades() const { return upgrades_; }

UnitType Building::GetUnit() const { return unit_; }
