#include "gamescene.h"

#include <QApplication>
#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QRandomGenerator>
#include <QScreen>
#include <functional>
#include <memory>

#include "drawer.h"
#include "planet.h"
#include "player.h"

GameScene::GameScene(QObject *parent) : QGraphicsScene(parent) {
  drawer_ = std::make_shared<Drawer>(this);
}

void GameScene::NewGame() {
  // TODO
  // Надо выбрать радиус
  int32_t width = qApp->screens()[0]->size().width();

  Planet *start_planet = new Planet(QPointF(0, 0), width / 16);
  std::shared_ptr<Planet> player_planet(start_planet);

  drawer_->DrawPlanet(player_planet);

  player_ = std::make_shared<Player>(player_planet);

  player_planet->SetOwner(player_);
  GenerateMap();

  // TODO
  //Здесь должна происходить генерация ботов и присвоение им планет
}

void GameScene::GenerateMap() {
  std::function<double(QGraphicsItem *, QPointF)> distance =
      [](QGraphicsItem *left, QPointF right) {
        return (left->pos().x() - right.x()) * (left->pos().x() - right.x()) +
               (left->pos().y() - right.y()) * (left->pos().y() - right.y());
      };

  int32_t width = qApp->screens()[0]->size().width();

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
      uint32_t distance_between = QRandomGenerator::global()->generate() %
                                      static_cast<uint32_t>(width / 16) +
                                  static_cast<uint32_t>(width / 7);

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
        Planet *new_planet = new Planet(coordinates, width / 16);
        std::shared_ptr<Planet> planet_ptr(new_planet);

        drawer_->DrawPlanet(planet_ptr);
        number_of_planets++;
      }
    }
  }
}
