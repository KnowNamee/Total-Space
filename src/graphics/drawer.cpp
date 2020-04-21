#include "graphics/drawer.h"

#include "scene/gamescene.h"
#include "scene/gameview.h"
#include "objects/planet.h"
#include "graphics/planetgraphics.h"

Drawer::Drawer(GameScene *game_scene) : QObject(), game_scene_(game_scene) {}

void Drawer::DrawPlanet(const std::shared_ptr<Planet> &planet) {
  PlanetGraphics *planet_item = new PlanetGraphics(
      planet, dynamic_cast<GameView *>(game_scene_->views()[0]));
  planet_item->setPos(planet->Coordinates());
  game_scene_->addItem(planet_item);
}
