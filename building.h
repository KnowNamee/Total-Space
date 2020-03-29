#ifndef BUILDING_H
#define BUILDING_H

#include <QObject>
#include <memory>

class Planet;

class Building : public QObject {
  Q_OBJECT
 public:
  Building();

 private:
  const std::shared_ptr<Planet> parent_;
};

#endif  // BUILDING_H
