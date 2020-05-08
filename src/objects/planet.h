#ifndef PLANET_H
#define PLANET_H

#include <QObject>
#include <QPoint>
#include <memory>
#include <set>

#include "util/utility.h"

class PlayerBase;
class Building;
class Unit;

class Planet : public QObject {
  Q_OBJECT
 public:
  Planet(QPointF coordinates, double radius);

  void SetOwner(PlayerBase* owner);

  void AddBuilding(BuildingType building);
  void AddUnit(UnitType unit);
  void AddUnits(const QVector<UnitType>& units);
  void RemoveUnit(UnitType unit);
  void RemoveUnits(const QVector<UnitType>& units);

  void Upgrade();

  int32_t GetBatteriesIncome() const;
  int32_t GetToolsIncome() const;
  const Resources& GetIncome() const;
  QPointF GetCoordinates() const;
  double GetRadius() const;
  const QVector<BuildingType>& GetBuildings() const;
  const QVector<UnitType>& GetUnits() const;
  PlayerBase* GetOwner() const;

  std::set<BuildingType> GetAvailableBuildings() const;
  std::set<UnitType> GetAvailableUnits() const;

  bool TakeAttack(const std::map<Planet*, QVector<UnitType>>& enemy_units);
  std::pair<int32_t, int32_t> CountPoints(const UnitCharacteristics& self,
                                          const UnitCharacteristics& enemy);
  bool Lose(const std::map<Planet*, QVector<UnitType>>& enemy_units);
  bool Draw(const std::map<Planet*, QVector<UnitType>>& enemy_units);
  bool Win(const std::map<Planet*, QVector<UnitType>>& enemy_units);
  void MoveUnits(const std::map<Planet*, QVector<UnitType>>& enemy_units);

 private:
  int32_t level_ = 1;
  const double radius_;
  const QPointF coordinates_;
  PlayerBase* owner_ = nullptr;
  Resources income_;
  QVector<BuildingType> buildings_;
  QVector<UnitType> units_on_planet_;
};

#endif  // PLANET_H
