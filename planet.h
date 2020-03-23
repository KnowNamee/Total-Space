#ifndef PLANET_H
#define PLANET_H

#include <QObject>
#include <QPoint>
#include <memory>

#include "playerbase.h"

class Building;
class Unit;

class Planet : public QObject {
  Q_OBJECT
 public:
  Planet(QPointF coordinates, qreal radius);

  void SetOwner(std::shared_ptr<PlayerBase> owner);

  QPointF Coordinates() const;
  qreal Radius() const;

 private:
  std::shared_ptr<PlayerBase> owner_;
  const QPointF coordinates_;
  const qreal radius_;
  QVector<std::shared_ptr<Building>> buildings_;
  QVector<std::shared_ptr<Unit>> units_on_planet_;
};

#endif  // PLANET_H
