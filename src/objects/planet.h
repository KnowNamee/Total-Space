#ifndef PLANET_H
#define PLANET_H

#include <QObject>
#include <QPoint>
#include <memory>

#include "util/utility.h"

class PlayerBase;
class Building;
class Unit;

class Planet : public QObject {
  Q_OBJECT
public:
  Planet(QPointF coordinates, double radius);

  void SetOwner(const std::shared_ptr<PlayerBase> &owner);

  void Build(BuildingType building);
  void AddUnit(UnitType unit);

  int32_t GetBatteriesIncome() const;
  int32_t GetToolsIncome() const;
  const Resources& GetIncome() const;

  QPointF GetCoordinates() const;
  double GetRadius() const;

private:
  Resources income_;
  std::shared_ptr<PlayerBase> owner_;
  const QPointF coordinates_;
  const double radius_;
  QVector<BuildingType> buildings_;
  QVector<UnitType> units_on_planet_;
};

#endif // PLANET_H
