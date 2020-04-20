#include "unit.h"
#include "util/utility.h"

Unit::Unit(const QString& caption, const int32_t& power, const Resources& cost)
    : caption_(caption), power_(power), cost_(cost) {}

const QString& Unit::GetCaption() const { return caption_; }
const Resources& Unit::GetCost() const { return cost_; }
const int32_t& Unit::GetPower() const { return power_; }
