#ifndef PLANET_H
#define PLANET_H

#include <QObject>
#include <QPoint>
#include <memory>

#include "playerbase.h"
#include "resources.h"

class Building;
class Unit;

class Planet : public QObject {
  Q_OBJECT
public:
  Planet(QPointF coordinates, double radius);

  void SetOwner(const std::shared_ptr<PlayerBase> &owner);

  void Build(std::shared_ptr<Building> building_ptr);

  int64_t GetBatteriesIncome() const;
  int64_t GetToolsIncome() const;

  QPointF Coordinates() const;
  double Radius() const;

private:
  Resources income_;
  std::shared_ptr<PlayerBase> owner_;
  const QPointF coordinates_;
  const double radius_;
  QVector<std::shared_ptr<Building>> buildings_;
  QVector<std::shared_ptr<Unit>> units_on_planet_;
};

#endif // PLANET_H
