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

  player_ = std::make_shared<Player>(player_planet, "#C9F76F");
  player_planet->SetOwner(player_.get());

  SetSceneSettings();
  GenerateMap();

  // Добавляем ботов
  bot1_ = std::make_shared<Bot>(graph_->GetBotPlanet(), "#023883");  // blue
  bot1_->GetPlanets()[0]->SetOwner(bot1_);
  bot2_ = std::make_shared<Bot>(graph_->GetBotPlanet(), "#D49000");  // orange
  bot2_->GetPlanets()[0]->SetOwner(bot2_);

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
      QRandomGenerator::global()->generate() % 10 + 20;

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
      bool is_allowed_distance = false;
      int32_t counter = 0;
      while (!is_allowed_distance && counter < 10000) {
        is_allowed_distance = true;
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

std::map<Planet*, QVector<UnitType>> GameScene::GetNearestUnits(
    PlayerBase* player) {
  Planet* planet = Controller::GetActivePlanet();
  if (planet == nullptr) {
    return {};
  }
  std::map<Planet*, QVector<UnitType>> nearby_units;
  for (const auto& nearby_planet : player->GetPlanets()) {
    if (Distance(nearby_planet->GetCoordinates(), planet->GetCoordinates()) <
        kMaximalDistance) {
      QVector<UnitType> planet_units = nearby_planet->GetUnits();
      if (planet_units.size() > 0) {
        nearby_units[nearby_planet.get()] = planet_units;
      }
    }
  }
  return nearby_units;
}

void GameScene::UpdatePlanetsGraph() { graph_->Update(); }
