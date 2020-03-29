#include "playerbase.h"

PlayerBase::PlayerBase(const std::shared_ptr<Planet>& planet) {
  planets_.push_back(planet);
}

int64_t PlayerBase::Money() const { return money_; }

const QVector<std::shared_ptr<Planet>>& PlayerBase::Planets() const {
  return planets_;
}

const QVector<std::shared_ptr<Unit>>& PlayerBase::Units() const {
  return units_;
}
