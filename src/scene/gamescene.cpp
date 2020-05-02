#include "scene/gamescene.h"

#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QRandomGenerator>

#include "core/planetsgraph.h"
#include "core/statemachine.h"
#include "data/loader.h"
#include "graphics/drawer.h"
#include "graphics/planetgraphics.h"
#include "objects/planet.h"
#include "objects/player.h"
#include "scene/gameview.h"

GameScene::GameScene(QObject* parent) : QGraphicsScene(parent) {
  drawer_ = std::make_shared<Drawer>(this);
}

void GameScene::Destroy() {
  QListIterator<QGraphicsItem*> it(Controller::scene->items());
  while (it.hasNext()) {
    Controller::scene->removeItem(it.next());
  }
  player_ = nullptr;
}

void GameScene::HideAll() {
  QListIterator<QGraphicsItem*> it(Controller::scene->items());
  while (it.hasNext()) {
    it.next()->hide();
  }
}

void GameScene::ShowAll() {
  QListIterator<QGraphicsItem*> it(Controller::scene->items());
  while (it.hasNext()) {
    it.next()->show();
  }
}

Player* GameScene::GetPlayer() const { return player_.get(); }

double GameScene::GetMapSize() const { return kMapSize; }

void GameScene::NewGame() {
  const double kWidth = views()[0]->sceneRect().width();

  // TODO
  // Надо выбрать радиус
  Planet* start_planet = new Planet(QPointF(0, 0), kWidth / 16 * 3);
  std::shared_ptr<Planet> player_planet(start_planet);
  drawer_->DrawPlanet(player_planet);

  player_ = std::make_shared<Player>(player_planet);

  player_planet->SetOwner(player_);
  SetSceneSettings();
  GenerateMap();

  // Добавляем ботов
  red_bot_ = std::make_shared<Bot>(Qt::red, graph_->GetBotPlanet());
  red_bot_->GetPlanets()[0]->SetOwner(red_bot_);
  Blue_bot_ = std::make_shared<Bot>(Qt::blue, graph_->GetBotPlanet());
  Blue_bot_->GetPlanets()[0]->SetOwner(Blue_bot_);

  // Перерисовываем рёбра графа
  UpdatePlanetsGraph();
}

void GameScene::SetSceneSettings() {
  background_ =
      new ImageItem(Loader::GetButtonImage(ButtonsEnum::kMainBackground),
                    static_cast<int32_t>(4 * kMapSize * kWidth),
                    static_cast<int32_t>(4 * kMapSize * kHeight));
  background_->setPos(0, 0);
  background_->setZValue(-5);
  Controller::scene->addItem(background_);
}

void GameScene::GenerateMap() {
  uint32_t required_number_of_planets =
      QRandomGenerator::global()->generate() % 10 + 25;

  const double kPlanetRadius = kWidth / 16 * 3;
  const double kSizeCoefficient = 0.7;
  const double kMapWidth =
      kSizeCoefficient * kMapSize * kWidth - kWidth / 2 + kPlanetRadius;
  const double kMapHeight =
      kSizeCoefficient * kMapSize * kHeight - kHeight / 2 + kPlanetRadius;
  const double kArea = 4 * kMapWidth * kMapHeight;
  const double kCellArea = kArea / required_number_of_planets;
  const double kWtoH = 1. * kWidth / kHeight;
  const double kCellWidth = std::sqrt(kCellArea * kWtoH);
  const double kCellHeight = std::sqrt(kCellArea / kWtoH);
  const double kMinimalDistance = 2 * kPlanetRadius;
  for (double x = -kMapWidth; x < kMapWidth; x += kCellWidth) {
    for (double y = -kMapHeight; y < kMapHeight; y += kCellHeight) {
      bool is_allowed_distance = true;
      int32_t counter = 0;
      while (is_allowed_distance && counter < 1000) {
        counter++;
        double left_x = std::max(-kMapWidth + kPlanetRadius, x);
        double right_x = x + kCellWidth;
        int64_t planet_x = QRandomGenerator::global()->generate() %
                               static_cast<int64_t>(right_x - left_x) +
                           static_cast<int64_t>(left_x);
        double top_y = std::max(-kMapHeight + kPlanetRadius, y);
        double bottom_y = std::min(kMapHeight - kPlanetRadius, y + kCellHeight);
        int64_t planet_y = QRandomGenerator::global()->generate() %
                               static_cast<int64_t>(bottom_y - top_y) +
                           static_cast<int64_t>(top_y);
        QPointF coordinates(planet_x, planet_y);
        for (QGraphicsItem* item : items()) {
          PlanetGraphics* another_planet = dynamic_cast<PlanetGraphics*>(item);
          if (another_planet == nullptr) {
            continue;
          }
          if (Distance(another_planet->pos(), coordinates) < kMinimalDistance) {
            is_allowed_distance = false;
            break;
          }
        }
        if (is_allowed_distance) {
          drawer_->DrawPlanet(
              std::make_shared<Planet>(coordinates, kPlanetRadius));
        }
      }
    }
  }

  graph_ = std::make_shared<PlanetsGraph>(items());
  drawer_->DrawPlanetsGraph(graph_);
}

double GameScene::Distance(const QPointF& lhs, const QPointF& rhs) {
  return std::sqrt((lhs.x() - rhs.x()) * (lhs.x() - rhs.x()) +
                   (lhs.y() - rhs.y()) * (lhs.y() - rhs.y()));
}

std::vector<UnitType> GameScene::GetNearestUnits() {
  Planet* planet = Controller::GetActivePlanet();
  std::vector<UnitType> nearby_units;
  for (QGraphicsItem* item : items()) {
    PlanetGraphics* planet_graphics = dynamic_cast<PlanetGraphics*>(item);
    if (planet_graphics == nullptr) {
      continue;
    }

    Planet* nearby_planet = planet_graphics->GetPlanet().get();
    if (Distance(nearby_planet->GetCoordinates(), planet->GetCoordinates()) <
        kMaximalDistance) {
      nearby_units.insert(nearby_units.end(), nearby_planet->GetUnits().begin(),
                          nearby_planet->GetUnits().end());
    }
  }
  return nearby_units;
}

void GameScene::UpdatePlanetsGraph() { graph_->Update(); }
