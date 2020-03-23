#include "gamescene.h"

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QRandomGenerator>
#include <functional>
#include <memory>

#include "drawer.h"
#include "planet.h"
#include "player.h"

GameScene::GameScene(QObject *parent) : QGraphicsScene(parent) {
  drawer_ = std::make_shared<Drawer>(std::shared_ptr<GameScene>(this));
}

void GameScene::NewGame() {
  Planet *start_planet = new Planet(QPointF(0, 0), 60);  // Надо выбрать радиус
  std::shared_ptr<Planet> player_planet(start_planet);

  drawer_->DrawPlanet(player_planet);

  player_ = std::make_shared<Player>(player_planet);

  player_planet->SetOwner(player_);
}

void GameScene::GenerateMap() {
  std::function<qreal(QGraphicsItem *, QPointF)> distance =
      [](QGraphicsItem *left, QPointF right) {
        return (left->pos().x() - right.x()) * (left->pos().x() - right.x()) +
               (left->pos().y() - right.y()) * (left->pos().y() - right.y());
      };

  int32_t number_of_planets = 0;
  int32_t required_number_of_planets =
      QRandomGenerator::global()->generate() % 10 + 20;

  while (number_of_planets < required_number_of_planets) {
    foreach (QGraphicsItem *planet, items()) {
      int32_t angle = QRandomGenerator::global()->generate() % 360;
      // Разбежку расстояний между планетами также нужно выбрать
      int32_t distance_between =
          QRandomGenerator::global()->generate() % 100 + 200;

      QPointF coordinates(
          planet->pos().x() + distance_between * cos(angle * M_PI / 180),
          planet->pos().y() + distance_between * sin(angle * M_PI / 180));

      bool is_allowed_distance = true;
      foreach (QGraphicsItem *another_planet, items()) {
        if (distance(another_planet, coordinates) < distance_between) {
          is_allowed_distance = false;
          break;
        }
      }

      if (is_allowed_distance) {
        // Надо выбрать радиус, возможно рандомный
        Planet *new_planet = new Planet(QPointF(0, 0), 60);
        std::shared_ptr<Planet> planet_ptr(new_planet);

        drawer_->DrawPlanet(planet_ptr);
        number_of_planets++;
      }
    }
  }
}
