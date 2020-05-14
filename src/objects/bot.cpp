#include "bot.h"

Bot::Bot(Planet* planet, const QString& color)
    : PlayerBase(planet, PlayerBase::Type::kBot, color) {}

Bot::~Bot() {}

void Bot::Next() {}

void Bot::ApplyAttackStrategy() {
  for (Planet* planet : GetPlanets()) {
    if (planet->IsBorder()) {
        ConsiderBorderPlanet();
    }
  }
}

void Bot::ConsiderBorderPlanet() {}
