#include "graphics/planetgraphics.h"

#include <QDebug>
#include <QPainter>
#include <QRandomGenerator>

#include "scene/gameview.h"
#include "data/loader.h"
#include "objects/planet.h"

PlanetGraphics::PlanetGraphics(const std::shared_ptr<Planet> &planet,
                               GameView *view)
    : planet_(planet), view_(view) {
  planet_image_ =
      Loader::GetPlanetImage(QRandomGenerator::global()->generate() % 3);
}

int PlanetGraphics::type() const { return Type; }

Planet *PlanetGraphics::GetPlanet() { return planet_.get(); }

QRectF PlanetGraphics::boundingRect() const {
  return QRectF(planet_->Coordinates().x() - planet_->Radius(),
                planet_->Coordinates().y() - planet_->Radius(),
                2 * planet_->Radius(), 2 * planet_->Radius());
}

void PlanetGraphics::paint(QPainter *painter,
                           const QStyleOptionGraphicsItem *option,
                           QWidget *widget) {
  painter->drawPixmap(
      static_cast<int>(planet_->Coordinates().x() - planet_->Radius()),
      static_cast<int>(planet_->Coordinates().y() - planet_->Radius()),
      static_cast<int>(2 * planet_->Radius()),
      static_cast<int>(2 * planet_->Radius()), *planet_image_, 0, 0, 1000,
      1000);

  Q_UNUSED(widget)
  Q_UNUSED(option)
}
