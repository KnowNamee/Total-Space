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
  Player(Planet* planet, const QString& color);
  virtual ~Player() override;

  void Next() override;
};

#endif  // PLAYER_H
