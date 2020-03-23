#include "drawer.h"

#include "gamescene.h"
#include "planetgraphics.h"

Drawer::Drawer(std::shared_ptr<GameScene> game_scene)
    : game_scene_(game_scene) {}

void Drawer::DrawPlanet(std::shared_ptr<Planet> planet) {
  PlanetGraphics *planet_item = new PlanetGraphics(planet);
  game_scene_->addItem(planet_item);
}
