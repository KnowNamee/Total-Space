#ifndef PLANET_H
#define PLANET_H

#include <QObject>
#include <QPoint>
class Building;
class Unit;

class Planet
{
//  Q_OBJECT
public:
  Planet();

private:
  QPointF coordinates_;
  QVector<Building*> buildings_;
  QVector<Unit*> units_on_planet_;

};

#endif // PLANET_H
