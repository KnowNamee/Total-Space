#ifndef BUILDING_H
#define BUILDING_H

#include <QObject>
#include <memory>

#include "util/utility.h"

class Building : public QObject {
  Q_OBJECT
 public:
  Building(const QString& caption, const QString& type,
           const QVector<BuildingType>& upgrades, UnitType unit,
           const Resources& cost, const Resources& income);
  const Resources& GetIncome() const;
  const QString& GetCaption() const;

 private:
  const QString caption_;
  const QString type_;
  const QVector<BuildingType> upgrades_;
  const UnitType unit_;
  const Resources cost_;
  const Resources income_;
};

#endif  // BUILDING_H
