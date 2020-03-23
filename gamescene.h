#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QGraphicsScene>
#include <QObject>
#include <memory>

class Drawer;
class Player;

class GameScene : public QGraphicsScene {
  Q_OBJECT
 public:
  GameScene(QObject *parent);
  void NewGame();

 private:
  void GenerateMap();

  std::shared_ptr<Drawer> drawer_;
  std::shared_ptr<Player> player_;
};

#endif  // GAMESCENE_H
