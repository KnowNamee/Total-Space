#include "objects/unit.h"

#include "util/utility.h"

Unit::Unit(const QString& caption, const Resources& cost, int32_t power,
           UnitRole role, UnitType enemy, UnitCharacteristics charachteristics)
    : caption_(caption),
      cost_(cost),
      power_(power),
      role_(role),
      enemy_(enemy),
      charachteristics_(charachteristics) {}

const QString& Unit::GetCaption() const { return caption_; }
const Resources& Unit::GetCost() const { return cost_; }

const UnitCharacteristics &Unit::GetUnitCharacteristics() const {
  return charachteristics_;
}

UnitRole Unit::GetUnitRole() const { return role_; }
UnitType Unit::GetUnitEnemy() const { return enemy_; }
int32_t Unit::GetPower() const { return power_; }
