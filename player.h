#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <memory>

#include "playerbase.h"

class Planet;
class Unit;

class Player : public PlayerBase {
 public:
  explicit Player(std::shared_ptr<Planet> planet);

  Player(QVector<std::shared_ptr<Planet>> planets, int64_t money,
         int32_t income);
};

#endif  // PLAYER_H
