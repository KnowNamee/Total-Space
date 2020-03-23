#include "player.h"

Player::Player(std::shared_ptr<Planet> planet) : PlayerBase(planet) {}

Player::Player(QVector<std::shared_ptr<Planet>> planets, int64_t money,
               int32_t income) {
  money_ = money;
  income_ = income;
  planets_ = planets;
}
