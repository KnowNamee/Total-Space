#include "planet.h"

Planet::Planet(QPointF coordinates, double radius)
    : coordinates_(coordinates), radius_(radius) {}

void Planet::SetOwner(const std::shared_ptr<PlayerBase>& owner) {
  owner_ = owner;
}

QPointF Planet::Coordinates() const { return coordinates_; }

double Planet::Radius() const { return radius_; }
