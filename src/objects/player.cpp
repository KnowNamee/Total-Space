#include "objects/player.h"

Player::Player(const std::shared_ptr<Planet>& planet, const QString& color)
    : PlayerBase(planet, PlayerBase::Type::kPlayer, color) {}
