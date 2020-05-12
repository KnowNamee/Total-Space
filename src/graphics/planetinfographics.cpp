#include "planetinfographics.h"

#include <core/statemachine.h>
#include <data/objectsstorage.h>
#include <objects/planet.h>

#include <QPainter>
#include <QPixmap>

PlanetInfoGraphics::PlanetInfoGraphics(QPixmap* planet_image, int32_t width,
                                       int32_t height)
    : planet_image_(planet_image),
      level_("level" +
             QString::number(Controller::GetActivePlanet()->GetLevel())),
      width_(width),
      height_(height) {
  units_to_data_ = Controller::GetActivePlanet()->GetUnitsToData();
}

int32_t PlanetInfoGraphics::GetWidth() const { return width_; }

QRectF PlanetInfoGraphics::boundingRect() const {
  return QRectF(0, 0, width_, height_);
}

void PlanetInfoGraphics::paint(QPainter* painter,
                               const QStyleOptionGraphicsItem* option,
                               QWidget* widget) {
  painter->drawPixmap(QRect(0, 0, width_ / 2, width_ / 2), *planet_image_);
  painter->setPen(QColor(Qt::white));
  //   TODO
  //   painter->setFont + размер в зависимости от скейла
  //   подобрать позицию текста
  int32_t level_text_x = width_ / 4 * 3 - width_ / 15;
  painter->drawText(level_text_x, 0, level_);
  int32_t unit_y = height_ / 5;
  for (const auto& unit_to_data : units_to_data_) {
    //   TODO
    //   отрисовка картинки юнита подобрать размеры
    painter->drawText(level_text_x - width_ / 15, unit_y,
                      unit_to_data.second.caption);
    painter->drawText(level_text_x + width_ / 15, unit_y,
                      QString::number(unit_to_data.second.quantity));
    unit_y += height_ / 5;
  }
  Q_UNUSED(option)
  Q_UNUSED(widget)
}
