#include "bot.h"

Bot::Bot(std::shared_ptr<Planet> planet, const QString& color)
    : PlayerBase(planet, PlayerBase::Type::kBot, color) {}
