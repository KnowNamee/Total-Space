#ifndef PLAYERBASE_H
#define PLAYERBASE_H

#include <QObject>
#include <memory>

#include "util/utility.h"

class Planet;
class Unit;

class PlayerBase {
 public:
  PlayerBase() = default;

  explicit PlayerBase(const std::shared_ptr<Planet>& planet, int type);

  const Resources& GetResources() const;
  int32_t GetTools() const;
  int32_t GetBatteries() const;

  void UpdateResources();

  void AddPlanet(const std::shared_ptr<Planet>& planet);

  const QVector<std::shared_ptr<Planet>>& GetPlanets() const;
  int64_t GetArmyPower() const;

  int Type();

  enum {
    kPlayer,
    kBot,
  };

 private:
  Resources resources_;
  int64_t army_power_;
  int type_;

  QVector<std::shared_ptr<Planet>> planets_;
};

#endif  // PLAYERBASE_H
