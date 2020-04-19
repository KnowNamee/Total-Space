#ifndef UNIT_H
#define UNIT_H

#include "utility.h"
#include <QObject>
#include <QString>

class Unit : public QObject {
  Q_OBJECT
public:
  Unit(const QString& caption, const int32_t& power, const Resources& cost);

  const QString& GetCaption() const;
  const Resources& GetCost() const;
  const int32_t& GetPower() const;

private:
  QString caption_;
  int32_t power_;
  Resources cost_;
};

#endif // UNIT_H
