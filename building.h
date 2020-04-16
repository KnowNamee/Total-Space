#ifndef BUILDING_H
#define BUILDING_H

#include <QObject>
#include <memory>

#include "utility.h"

class EconomicBuilding : public QObject {
  Q_OBJECT
 public:
  EconomicBuilding(const QString& caption, const QString& type,
                   const Resources& cost, const Resources& income,
                   const Resources& keeping);
  const Resources& GetIncome() const;
  const QString& GetCaption() const;

 private:
  const QString caption_;
  const QString type_;
  const Resources cost_;
  const Resources income_;
  const Resources keeping_;
};

class WarBuilding : public QObject {
  Q_OBJECT
 public:
  WarBuilding();
};

#endif  // BUILDING_H
