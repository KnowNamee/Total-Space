#ifndef BUILDING_H
#define BUILDING_H

#include "resources.h"
#include <QObject>
#include <memory>

class Planet;

class Building : public QObject {
  Q_OBJECT
public:
  Building();

  int64_t GetBatteriesIncome() const;
  int64_t GetToolsIncome() const;

private:
  const std::shared_ptr<Planet> parent_;
  Resources income_;
};

#endif // BUILDING_H
