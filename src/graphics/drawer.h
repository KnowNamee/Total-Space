#ifndef DRAWER_H
#define DRAWER_H

#include <QObject>
#include <memory>

class GameScene;
class Planet;
class PlanetsGraph;

class Drawer : public QObject {
  Q_OBJECT
 public:
  Drawer(GameScene* game_scene);

  void DrawPlanet(const std::shared_ptr<Planet>& planet);
  void DrawPlanetsGraph(const std::shared_ptr<PlanetsGraph>& graph);

 private:
  GameScene* game_scene_;
};

#endif  // DRAWER_H
