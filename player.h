#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
class Planet;
class Unit;


class Player
{
public:
  Player();

  int64_t Money();
  QVector<Planet*> Planets();
  QVector<Unit*> Units();

private:
  int64_t money_;
  int32_t income_;
  QVector<Planet*> planets_;
  QVector<Unit*> units_;

};

#endif // PLAYER_H
