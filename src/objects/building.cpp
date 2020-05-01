#include "objects/building.h"

Building::Building(const QString& caption, const QString& type,
                   const QVector<BuildingType>& upgrades, UnitType unit,
                   const Resources& cost, const Resources& income)
    : caption_(caption),
      type_(type),
      upgrades_(upgrades),
      unit_(unit),
      cost_(cost),
      income_(income) {}

const Resources& Building::GetIncome() const { return income_; }

const QString& Building::GetCaption() const { return caption_; }

const Resources& Building::GetCost() const { return cost_;}
