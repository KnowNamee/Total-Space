#include "objects/planet.h"
#include "objects/playerbase.h"
#include "objects/building.h"
#include "objects/unit.h"

#include "data/objectsstorage.h"

Planet::Planet(QPointF coordinates, double radius)
    : coordinates_(coordinates), radius_(radius) {}

void Planet::SetOwner(const std::shared_ptr<PlayerBase>& owner) {
  owner_ = owner;
}

const Resources& Planet::GetIncome() const { return income_; }

 void Planet::Build(BuildingType building) {
  owner_->SubResources(ObjectsStorage::GetBuilding(building)->GetCost());
  buildings_.push_back(building);
  income_ += ObjectsStorage::GetIncome(building);
 }

 void Planet::AddUnit(UnitType unit) {
   owner_->SubResources(ObjectsStorage::GetUnit(unit)->GetCost());
   units_on_planet_.push_back(unit);
 }

int32_t Planet::GetToolsIncome() const { return income_.GetTools(); }
int32_t Planet::GetBatteriesIncome() const { return income_.GetBatteries(); }

QPointF Planet::GetCoordinates() const { return coordinates_; }

double Planet::GetRadius() const { return radius_; }
