#include "planet.h"

#include "building.h"

Planet::Planet(QPointF coordinates, double radius)
    : coordinates_(coordinates), radius_(radius) {}

void Planet::SetOwner(const std::shared_ptr<PlayerBase> &owner) {
  owner_ = owner;
}

const Resources &Planet::GetIncome() const { return income_; }

// TODO
// void Planet::Build(std::shared_ptr<Building> building_ptr) {
//  buildings_.push_back(building_ptr);
//  income_.tools += building_ptr->GetToolsIncome();
//  income_.batteries += building_ptr->GetBatteriesIncome();
//}

int32_t Planet::GetToolsIncome() const { return income_.GetTools(); }
int32_t Planet::GetBatteriesIncome() const { return income_.GetBatteries(); }

QPointF Planet::Coordinates() const { return coordinates_; }

double Planet::Radius() const { return radius_; }
