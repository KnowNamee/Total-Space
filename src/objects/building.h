#ifndef BUILDING_H
#define BUILDING_H

#include <QObject>
#include <memory>

#include "util/utility.h"

class Building : public QObject {
  Q_OBJECT
 public:
  Building(const QString& caption, BuildingRole role,
           const QVector<BuildingType>& upgrades, int32_t level, int32_t time,
           UnitType unit, const Resources& cost, const Resources& income);
  const Resources& GetIncome() const;
  const QString& GetCaption() const;
  BuildingRole GetBuildingRole() const;
  int32_t GetLevel() const;
  int32_t GetTime() const;
  const Resources& GetCost() const;
  const QVector<BuildingType>& GetUpgrades() const;
  UnitType GetUnit() const;

 private:
  const QString caption_;
  BuildingRole role_;
  const QVector<BuildingType> upgrades_;
  const int32_t level_;
  const int32_t time_;
  const UnitType unit_;
  const Resources cost_;
  const Resources income_;
};

#endif  // BUILDING_H
