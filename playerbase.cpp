#include "playerbase.h"
#include "planet.h"

PlayerBase::PlayerBase(const std::shared_ptr<Planet> &planet) {
  planets_.push_back(planet);
}

int64_t PlayerBase::Tools() const { return resources_.tools; }
int64_t PlayerBase::Battaries() const { return resources_.batteries; }

void PlayerBase::UpdateResources() {
  for (auto planet : Planets()) {
    resources_.tools += planet->GetToolsIncome();
    resources_.batteries += planet->GetBatteriesIncome();
  }
}

const QVector<std::shared_ptr<Planet>> &PlayerBase::Planets() const {
  return planets_;
}

const QVector<std::shared_ptr<Unit>> &PlayerBase::Units() const {
  return units_;
}
