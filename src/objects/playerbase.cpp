#include "playerbase.h"

#include <QDebug>

#include "objects/planet.h"

PlayerBase::PlayerBase(Planet* planet) { planets_.push_back(planet); }
PlayerBase::PlayerBase(Planet* planet, Type type, const QString& color)
    : type_(type), color_(color) {
  planets_.push_back(planet);
}

const Resources& PlayerBase::GetResources() const { return resources_; }

int32_t PlayerBase::GetTools() const { return resources_.GetTools(); }
int32_t PlayerBase::GetBatteries() const { return resources_.GetBatteries(); }

const QString& PlayerBase::GetName() const { return name_; }

void PlayerBase::SetName(const QString& name) { name_ = name; }

void PlayerBase::UpdateResources() {
  for (const auto& planet : GetPlanets()) {
    resources_ += planet->GetIncome();
  }
}

void PlayerBase::AddPlanet(Planet* planet) { planets_.push_back(planet); }

void PlayerBase::RemovePlanet(Planet* planet) { planets_.removeOne(planet); }

int64_t PlayerBase::GetArmyPower() const { return army_power_; }

void PlayerBase::IncreasePower(int32_t power) { army_power_ += power; }

const QVector<Planet*>& PlayerBase::GetPlanets() const { return planets_; }

PlayerBase::Type PlayerBase::GetType() { return type_; }

const QString& PlayerBase::GetColor() { return color_; }
