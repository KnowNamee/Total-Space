#ifndef PLAYERBASE_H
#define PLAYERBASE_H

#include <QObject>
#include <memory>

class Planet;
class Unit;

class PlayerBase {
 public:
  PlayerBase() = default;

  explicit PlayerBase(std::shared_ptr<Planet> planet);

  int64_t Money() const;
  QVector<std::shared_ptr<Planet>> Planets() const;
  QVector<std::shared_ptr<Unit>> Units() const;

 protected:
  int64_t money_;
  int32_t income_;
  QVector<std::shared_ptr<Planet>> planets_;
  QVector<std::shared_ptr<Unit>> units_;
};

#endif  // PLAYERBASE_H
