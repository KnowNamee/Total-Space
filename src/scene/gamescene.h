#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QGraphicsScene>
#include <QMouseEvent>
#include <QObject>
#include <memory>

#include "graphics/imageitem.h"

class Drawer;
class Player;

class GameScene : public QGraphicsScene {
  Q_OBJECT
 public:
  GameScene(QObject* parent = nullptr);

  void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

  void Destroy();
  void HideAll();
  void ShowAll();

 public slots:
  void NewGame();

 private:
  void SetSceneSettings();
  void GenerateMap();
  ImageItem* background;

  std::shared_ptr<Drawer> drawer_;
  std::shared_ptr<Player> player_;
};

#endif  // GAMESCENE_H
