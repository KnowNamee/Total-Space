#include "bot.h"

Bot::Bot(Planet* planet, const QString& color)
    : PlayerBase(planet, PlayerBase::Type::kBot, color) {}
