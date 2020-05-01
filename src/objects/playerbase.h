#ifndef PLAYERBASE_H
#define PLAYERBASE_H

#include "util/utility.h"
#include <QObject>
#include <memory>

class Planet;
class Unit;

class PlayerBase {
public:
  PlayerBase() = default;

  explicit PlayerBase(const std::shared_ptr<Planet>& planet);

  const Resources& GetResources() const;
  int32_t GetTools() const;
  int32_t GetBatteries() const;

  void AddResources(const Resources&);
  void SubResources(const Resources&);
  void UpdateResources();

  void AddPlanet(const std::shared_ptr<Planet>& planet);

  const QVector<std::shared_ptr<Planet>>& GetPlanets() const;
  int64_t GetArmyPower() const;

private:
  Resources resources_;
  int64_t army_power_;

  QVector<std::shared_ptr<Planet>> planets_; 
};

#endif // PLAYERBASE_H
