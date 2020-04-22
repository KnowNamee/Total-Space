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
  return QRectF(planet_->GetCoordinates().x() - planet_->GetRadius(),
                planet_->GetCoordinates().y() - planet_->GetRadius(),
                2 * planet_->GetRadius(), 2 * planet_->GetRadius());
}

void PlanetGraphics::paint(QPainter *painter,
                           const QStyleOptionGraphicsItem *option,
                           QWidget *widget) {
  painter->drawPixmap(
      static_cast<int>(planet_->GetCoordinates().x() - planet_->GetRadius()),
      static_cast<int>(planet_->GetCoordinates().y() - planet_->GetRadius()),
      static_cast<int>(2 * planet_->GetRadius()),
      static_cast<int>(2 * planet_->GetRadius()), *planet_image_, 0, 0, 1000,
      1000);

  Q_UNUSED(widget)
  Q_UNUSED(option)
}
