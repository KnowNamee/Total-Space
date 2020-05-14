#ifndef BOT_H
#define BOT_H

#include <QGraphicsItem>
#include <QList>
#include <memory>

#include "planet.h"
#include "playerbase.h"

class Bot : public PlayerBase {
 public:
  Bot() = delete;
  Bot(Planet* planet, const QString& color);
  virtual ~Bot() override;

  void Next() override;

 private:
  void ApplyAttackStrategy();
  void ConsiderBorderPlanet();
};

#endif  // BOT_H
