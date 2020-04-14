#include "planet.h"
#include "building.h"

Planet::Planet(QPointF coordinates, double radius)
    : coordinates_(coordinates), radius_(radius) {}

void Planet::SetOwner(const std::shared_ptr<PlayerBase> &owner) {
  owner_ = owner;
}

void Planet::Build(std::shared_ptr<Building> building) {
  buildings_.push_back(building);
  income_.tools += building->GetToolsIncome();
  income_.batteries += building->GetBatteriesIncome();
}

int64_t Planet::GetToolsIncome() const { return income_.tools; }
int64_t Planet::GetBatteriesIncome() const { return income_.batteries; }

QPointF Planet::Coordinates() const { return coordinates_; }

double Planet::Radius() const { return radius_; }
