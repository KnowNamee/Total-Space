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

  void UpdateResources();

  void AddPlanet(const std::shared_ptr<Planet>& planet);
  void RemovePlanet(const std::shared_ptr<Planet>& planet);

  const QVector<std::shared_ptr<Planet>>& GetPlanets() const;
  int64_t GetArmyPower() const;
  void IncreasePower(int32_t power);

private:
  Resources resources_;
  int64_t army_power_ = 0;

  QVector<std::shared_ptr<Planet>> planets_; 
};

#endif // PLAYERBASE_H
