#include "planetinfographics.h"

#include <QDebug>
#include <QFontDatabase>
#include <QPainter>
#include <QPixmap>

#include "core/statemachine.h"
#include "data/loader.h"
#include "data/objectsstorage.h"
#include "objects/planet.h"
#include "scene/gameview.h"

PlanetInfoGraphics::PlanetInfoGraphics(QPixmap* planet_image, int32_t width,
                                       int32_t height)
    : planet_image_(planet_image),
      level_("level\t" +
             QString::number(Controller::GetActivePlanet()->GetLevel())),
      width_(width),
      height_(height),
      font_(Loader::GetFont()) {
  units_to_data_ = Controller::GetActivePlanet()->GetUnitsToData();
}

int32_t PlanetInfoGraphics::GetWidth() const { return width_; }

QRectF PlanetInfoGraphics::boundingRect() const {
  return QRectF(0, 0, width_, height_);
}
void PlanetInfoGraphics::paint(QPainter* painter,
                               const QStyleOptionGraphicsItem* option,
                               QWidget* widget) {
  const double kScale = Controller::view->matrix().m11();
  painter->drawPixmap(
      QRect(0, 0, width_ / 2 + width_ / 10, width_ / 2 + width_ / 10),
      *planet_image_);
  painter->setPen(QColor(Qt::white));

  QFont fabulist_header =
      QFont(QFontDatabase::applicationFontFamilies(font_).first(),
            static_cast<int32_t>(37 / kScale));
  QFont fabulist_general =
      QFont(QFontDatabase::applicationFontFamilies(font_).first(),
            static_cast<int32_t>(26 / kScale));
  painter->setFont(fabulist_header);

  int32_t level_text_x = width_ / 4 * 3 - width_ / 30;
  painter->drawText(level_text_x, height_ / 10, level_);
  int32_t unit_y = height_ / 4;
  painter->setFont(fabulist_general);
  for (const auto& unit_to_data : units_to_data_) {
    painter->drawText(level_text_x - width_ / 15, unit_y,
                      unit_to_data.second.caption);
    painter->drawText(level_text_x + width_ / 15, unit_y,
                      QString::number(unit_to_data.second.quantity));

    if (unit_to_data.second.unit_image != nullptr) {
      painter->drawPixmap(
          QRect(level_text_x + 2 * width_ / 15, unit_y - height_ / 15,
                width_ / 12, width_ / 12),
          *unit_to_data.second.unit_image);
    }

    unit_y += height_ / 7;
  }
  Q_UNUSED(option)
  Q_UNUSED(widget)
}
