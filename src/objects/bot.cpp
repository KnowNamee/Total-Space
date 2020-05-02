#include "bot.h"

Bot::Bot(int type, std::shared_ptr<Planet> planet)
    : PlayerBase(planet, PlayerBase::kBot), color_(type) {}

int Bot::Color() { return color_; }
