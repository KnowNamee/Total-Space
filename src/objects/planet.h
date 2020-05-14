#ifndef PLANET_H
#define PLANET_H

#include <QObject>
#include <QPoint>
#include <memory>
#include <set>

#include "util/utility.h"

class PlayerBase;
class PlanetGraphics;
class Building;
class Unit;

class Planet : public QObject {
  Q_OBJECT
 private:
  enum class AttackResult {
    kWin,
    kLose,
    kDraw,
  };

 public:
  Planet(QPointF coordinates, double radius);

  void SetOwner(PlayerBase* owner);

  void AddBuilding(BuildingType building);
  void AddUnit(UnitType unit);
  void AddUnits(const QVector<UnitType>& units);
  void RemoveUnit(UnitType unit);
  void RemoveUnits(const QVector<UnitType>& units);

  void Upgrade();
  void Next();

  int32_t GetBatteriesIncome() const;
  int32_t GetToolsIncome() const;
  const Resources& GetIncome() const;
  Resources GetUpgradeCost() const;
  QPointF GetCoordinates() const;
  int32_t GetPower() const;
  double GetRadius() const;
  int32_t GetLevel() const;
  const QVector<BuildingType>& GetBuildings() const;
  const QVector<UnitType>& GetUnits() const;
  const QVector<UnitType>& GetTiredUnits() const;
  std::map<UnitType, UnitData> GetUnitsToData() const;
  PlanetGraphics* GetPlanetGraphics() const;
  QVector<Planet*> GetNearestPlanets() const;
  PlayerBase* GetOwner() const;

  std::set<BuildingType> GetAvailableBuildings() const;
  std::set<UnitType> GetAvailableUnits() const;

  bool IsBorder() const;
  std::map<Planet*, QVector<UnitType>> GetNearestNonBorderUnits() const;
  QVector<UnitType> GetMostProfitableUnits(const QVector<UnitType>& units,
                                           Resources resources) const;
  std::set<UnitType> GetAffordableUnits(const Resources& resources) const;

  bool TakeAttack(const std::map<Planet*, QVector<UnitType>>& enemy_units);
  AttackResult CalculateAttack(
      const std::map<Planet*, QVector<UnitType>>& enemy_units);
  std::pair<int32_t, int32_t> CountPoints(const UnitCharacteristics& self,
                                          const UnitCharacteristics& enemy);
  bool Lose(const std::map<Planet*, QVector<UnitType>>& enemy_units);
  bool Draw(const std::map<Planet*, QVector<UnitType>>& enemy_units,
            const std::pair<int32_t, int32_t>& points);
  bool Win(const std::map<Planet*, QVector<UnitType>>& enemy_units,
           const std::pair<int32_t, int32_t>& points);
  void MoveUnits(const std::map<Planet*, QVector<UnitType>>& enemy_units);

 private:
  std::pair<UnitType, UnitType> GetMaxAndMin(
      const std::map<UnitType, int32_t>& units_to_quantity,
      const std::set<UnitType>& types) const;

  int32_t level_ = 1;
  const double radius_;
  const QPointF coordinates_;
  PlayerBase* owner_ = nullptr;
  Resources income_;
  std::pair<int32_t, int32_t> attack_points_;
  QVector<BuildingType> buildings_;
  QVector<UnitType> units_on_planet_;
  QVector<UnitType> tired_units_;
};

#endif  // PLANET_H
