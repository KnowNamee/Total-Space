#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <memory>

#include "playerbase.h"

class Planet;
class Unit;

class Player : public PlayerBase {
public:
  explicit Player(Planet* planet);
  explicit Player(Planet* planet, const QString& color);
};

#endif  // PLAYER_H
