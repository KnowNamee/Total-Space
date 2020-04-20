#include "building.h"

EconomicBuilding::EconomicBuilding(const QString &caption, const QString &type,
                                   const Resources &cost,
                                   const Resources &income,
                                   const Resources &keeping)
    : caption_(caption),
      type_(type),
      cost_(cost),
      income_(income),
      keeping_(keeping) {}

const QString& EconomicBuilding::GetCaption() const { return caption_; }

WarBuilding::WarBuilding() {}
