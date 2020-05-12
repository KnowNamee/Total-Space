#include "planetinfographics.h"

#include <QDebug>
#include <QFontDatabase>
#include <QPainter>
#include <QPixmap>

#include "core/statemachine.h"
#include "data/loader.h"
#include "data/objectsstorage.h"
#include "objects/planet.h"

PlanetInfoGraphics::PlanetInfoGraphics(QPixmap* planet_image, int32_t width,
                                       int32_t height)
    : planet_image_(planet_image),
      level_("level\t" +
             QString::number(Controller::GetActivePlanet()->GetLevel())),
      font_(QFontDatabase::addApplicationFont(":/Img/Fabulist.ttf")),
      width_(width),
      height_(height) {
  QVector<UnitType> units = Controller::GetActivePlanet()->GetUnits();
  for (UnitType unit : units) {
    if (units_to_data_[unit].quantity == 0) {
      units_to_data_[unit].unit_image = Loader::GetUnitImage(unit);
      units_to_data_[unit].caption = ObjectsStorage::GetUnitCaption(unit);
    }
    units_to_data_[unit].quantity++;
  }
}

int32_t PlanetInfoGraphics::GetWidth() const { return width_; }

QRectF PlanetInfoGraphics::boundingRect() const {
  return QRectF(0, 0, width_, height_);
}
void PlanetInfoGraphics::paint(QPainter* painter,
                               const QStyleOptionGraphicsItem* option,
                               QWidget* widget) {
  painter->drawPixmap(
      QRect(0, 0, width_ / 2 + width_ / 10, width_ / 2 + width_ / 10),
      *planet_image_);
  painter->setPen(QColor(Qt::white));

  QFont fabulist_header = QFont(
      QFontDatabase::applicationFontFamilies(font_).first(), height_ / 20);
  QFont fabulist_general = QFont(
      QFontDatabase::applicationFontFamilies(font_).first(), height_ / 25);
  painter->setFont(fabulist_header);

  int32_t level_text_x = width_ / 4 * 3 - width_ / 30;
  painter->drawText(level_text_x, height_ / 10, level_);
  int32_t unit_y = height_ / 4;
  painter->setFont(fabulist_general);
  for (const auto& unit_to_data : units_to_data_) {
    //   отрисовка картинки юнита подобрать размеры
    painter->drawText(level_text_x - width_ / 15, unit_y,
                      unit_to_data.second.caption);
    painter->drawText(level_text_x + width_ / 15, unit_y,
                      QString::number(unit_to_data.second.quantity));

    if (!unit_to_data.second.unit_image->isNull()) {
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
