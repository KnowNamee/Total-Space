#ifndef BOT_H
#define BOT_H

#include <QGraphicsItem>
#include <QList>
#include <memory>

#include "objects/planet.h"
#include "objects/playerbase.h"
#include "util/utility.h"

class Bot : public PlayerBase {
 public:
  Bot() = delete;
  Bot(Planet* planet, const QString& color);
  virtual ~Bot() override;

  void Next() override;

 private:
  void ApplyAttackStrategy();
  void TryAttack(Planet* planet, Resources* available_resources);
  void TryWarBuild(Planet* planet, Resources* available_resources);
  void RunFromPlanet(Planet* planet, std::set<Planet*> planets_to_run);
  Resources BinarySearchResources(
      std::function<bool(Planet*, QVector<UnitType>, Planet*)>,
      const QVector<UnitType>&, Planet*, const Resources&,
      Planet* aim = nullptr);

  const double kAttackResources = 0.7;
  const double kUpgradeCoefficient = 0.5;
};

#endif  // BOT_H
