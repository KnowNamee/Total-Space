#include "playerbase.h"

#include "planet.h"

PlayerBase::PlayerBase(const std::shared_ptr<Planet> &planet) {
  planets_.push_back(planet);
}

const Resources &PlayerBase::GetResources() const { return resources_; }

int32_t PlayerBase::GetTools() const { return resources_.GetTools(); }
int32_t PlayerBase::GetBatteries() const { return resources_.GetBatteries(); }

void PlayerBase::UpdateResources() {
  for (auto planet : Planets()) {
    resources_ += planet->GetIncome();
  }
}

void PlayerBase::AddPlanet(std::shared_ptr<Planet> planet) {
  planets_.push_back(planet);
}

const QVector<std::shared_ptr<Planet>> &PlayerBase::Planets() const {
  return planets_;
}

const QVector<std::shared_ptr<Unit>> &PlayerBase::Units() const {
  return units_;
}
