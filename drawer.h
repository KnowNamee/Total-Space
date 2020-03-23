#ifndef DRAWER_H
#define DRAWER_H

#include <QObject>
#include <memory>

class GameScene;
class Planet;

class Drawer : public QObject {
  Q_OBJECT
 public:
  Drawer(std::shared_ptr<GameScene> game_scene);

  void DrawPlanet(std::shared_ptr<Planet> planet);

 private:
  std::shared_ptr<GameScene> game_scene_;
};

#endif  // DRAWER_H
