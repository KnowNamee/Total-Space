#include "graphics/drawer.h"

#include "scene/gamescene.h"
#include "scene/gameview.h"
#include "objects/planet.h"
#include "graphics/planetgraphics.h"

Drawer::Drawer(GameScene* game_scene) : QObject(), game_scene_(game_scene) {}

void Drawer::DrawPlanet(Planet* planet) {
  PlanetGraphics* planet_item = new PlanetGraphics(planet);
  planet_item->setPos(planet->GetCoordinates());
  game_scene_->addItem(planet_item);
}
