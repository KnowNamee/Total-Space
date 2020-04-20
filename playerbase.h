#ifndef PLAYERBASE_H
#define PLAYERBASE_H

#include "utility.h"
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

  const QVector<std::shared_ptr<Planet>>& Planets() const;
  const QVector<std::shared_ptr<Unit>>& Units() const;

protected:
  Resources resources_;

  QVector<std::shared_ptr<Planet>> planets_;
  QVector<std::shared_ptr<Unit>> units_;
};

#endif // PLAYERBASE_H
