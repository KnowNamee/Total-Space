#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <memory>

#include "playerbase.h"

class Planet;
class Unit;

class Player : public PlayerBase {
 public:
  explicit Player(const std::shared_ptr<Planet>& planet, const QString& color);
};

#endif  // PLAYER_H
