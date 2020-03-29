#include "gamescene.h"

#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QRandomGenerator>
#include <functional>
#include <memory>

#include "drawer.h"
#include "planet.h"
#include "player.h"

GameScene::GameScene(QObject *parent) : QGraphicsScene(parent) {
  SetSceneSettings();
  drawer_ = std::make_shared<Drawer>(this);
}

void GameScene::NewGame() {
  // TODO
  // Надо выбрать радиус
  Planet *start_planet = new Planet(QPointF(0, 0), 120);
  std::shared_ptr<Planet> player_planet(start_planet);

  drawer_->DrawPlanet(player_planet);

  player_ = std::make_shared<Player>(player_planet);

  player_planet->SetOwner(player_);
  GenerateMap();

  // TODO
  //Здесь должна происходить генерация ботов и присвоение им планет
}

void GameScene::SetSceneSettings()
{
  // TODO установка background и т.п. как настройки
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
      if (number_of_planets > required_number_of_planets) {
        break;
      }

      int32_t angle = QRandomGenerator::global()->generate() % 360;
      // TODO
      // Разбежку расстояний между планетами также нужно выбрать
      int32_t distance_between =
          QRandomGenerator::global()->generate() % 200 + 400;

      QPointF coordinates(
          planet->pos().x() + distance_between * cos(angle * M_PI / 180),
          planet->pos().y() + distance_between * sin(angle * M_PI / 180));

      bool is_allowed_distance = true;
      foreach (QGraphicsItem *another_planet, items()) {
        if (distance(another_planet, coordinates) <
            distance_between * distance_between) {
          is_allowed_distance = false;
          break;
        }
      }

      if (is_allowed_distance) {
        // TODO
        // Надо выбрать радиус, возможно рандомный
        Planet *new_planet = new Planet(coordinates, 120);
        std::shared_ptr<Planet> planet_ptr(new_planet);

        drawer_->DrawPlanet(planet_ptr);
        number_of_planets++;
      }
      qDebug() << number_of_planets;
    }
  }
}
