#include "playerbase.h"

#include "objects/planet.h"

PlayerBase::PlayerBase(const std::shared_ptr<Planet>& planet) {
  planets_.push_back(planet);
}

const Resources& PlayerBase::GetResources() const { return resources_; }

int32_t PlayerBase::GetTools() const { return resources_.GetTools(); }
int32_t PlayerBase::GetBatteries() const { return resources_.GetBatteries(); }

void PlayerBase::UpdateResources() {
  for (const auto& planet : GetPlanets()) {
    resources_ += planet->GetIncome();
  }
}

void PlayerBase::AddPlanet(const std::shared_ptr<Planet>& planet) {
  planets_.push_back(planet);
}

void PlayerBase::RemovePlanet(const std::shared_ptr<Planet> &planet) {
  planets_.removeOne(planet);
}

int64_t PlayerBase::GetArmyPower() const { return army_power_; }

void PlayerBase::IncreasePower(int32_t power) { army_power_+=power; }

const QVector<std::shared_ptr<Planet>>& PlayerBase::GetPlanets() const {
  return planets_;
}
