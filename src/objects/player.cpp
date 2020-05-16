#include "objects/player.h"

Player::Player(Planet* planet) : PlayerBase(planet) {}
Player::Player(Planet* planet, const QString& color)
    : PlayerBase(planet, PlayerBase::Type::kPlayer, color) {}

Player::~Player() {}

void Player::Next() {
  UpdateResources();
}
