#include "playerbase.h"

PlayerBase::PlayerBase(std::shared_ptr<Planet> planet) {
  planets_.push_back(planet);
}

int64_t PlayerBase::Money() const { return money_; }

QVector<std::shared_ptr<Planet>> PlayerBase::Planets() const {
  return planets_;
}

QVector<std::shared_ptr<Unit>> PlayerBase::Units() const { return units_; }
