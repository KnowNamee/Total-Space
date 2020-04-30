#ifndef UNIT_H
#define UNIT_H

#include <QObject>
#include <QString>

#include "util/utility.h"

class Unit : public QObject {
  Q_OBJECT
 public:
  Unit(const QString& caption, const Resources& cost, int32_t power,
       UnitRole role, UnitType enemy, UnitCharacteristics charachteristics);

  const QString& GetCaption() const;
  const Resources& GetCost() const;
  int32_t GetPower() const;
  const UnitCharacteristics& GetUnitCharacteristics() const;
  UnitRole GetUnitRole() const;
  UnitType GetUnitEnemy() const;

 private:
  QString caption_;
  Resources cost_;
  int32_t power_;
  UnitRole role_;
  UnitType enemy_;
  UnitCharacteristics charachteristics_;
};

#endif  // UNIT_H
