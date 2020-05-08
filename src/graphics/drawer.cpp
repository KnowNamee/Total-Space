#include "graphics/drawer.h"

#include "core/planetsgraph.h"
#include "graphics/planetgraphics.h"
#include "objects/planet.h"
#include "scene/gamescene.h"
#include "scene/gameview.h"

Drawer::Drawer(GameScene* game_scene) : QObject(), game_scene_(game_scene) {}

void Drawer::DrawPlanet(const std::shared_ptr<Planet>& planet) {
  PlanetGraphics* planet_item = new PlanetGraphics(planet);
  planet_item->setPos(planet->GetCoordinates());
  game_scene_->addItem(planet_item);
}

void Drawer::DrawPlanetsGraph(const std::shared_ptr<PlanetsGraph>& graph) {
  graph->Draw();
}
