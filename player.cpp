#include "player.h"

Player::Player()
{

}

int64_t Player::Money()
{
  return money_;
}

QVector<Planet *> Player::Planets()
{
  return planets_;
}

QVector<Unit*> Player::Units()
{
  return units_;
}
