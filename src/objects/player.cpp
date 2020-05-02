#include "objects/player.h"

Player::Player(const std::shared_ptr<Planet>& planet)
    : PlayerBase(planet, PlayerBase::kPlayer) {}
