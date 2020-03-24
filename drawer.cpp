#include "drawer.h"

#include "gamescene.h"
#include "planetgraphics.h"
#include "planet.h"

Drawer::Drawer(GameScene *game_scene)
    : QObject(), game_scene_(game_scene) {}

void Drawer::DrawPlanet(std::shared_ptr<Planet> planet) {
  PlanetGraphics *planet_item = new PlanetGraphics(planet);
  planet_item->setPos(planet->Coordinates());
  game_scene_->addItem(planet_item);
}

