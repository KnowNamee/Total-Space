#include "attackresultwindow.h"

#include <QFontDatabase>
#include <QPainter>

#include "core/statemachine.h"
#include "data/loader.h"
#include "data/objectsstorage.h"
#include "scene/gameview.h"

AttackResultWindow::AttackResultWindow(
    const std::map<UnitType, int32_t>& units_to_quantity, const QString& result,
    const QString& caption, int32_t width, int32_t height)
    : units_to_quantity_(units_to_quantity),
      result_(result),
      caption_(caption),
      width_(width),
      height_(height),
      font_(Loader::GetFont()) {}

QRectF AttackResultWindow::boundingRect() const {
  return QRectF(0, 0, width_, height_);
}

void AttackResultWindow::paint(QPainter* painter,
                               const QStyleOptionGraphicsItem* option,
                               QWidget* widget) {
  painter->drawPixmap(boundingRect().toAlignedRect(),
                      *Loader::GetButtonImage(ButtonsEnum::kMenuBackground));
  const double kScale = Controller::view->matrix().m11();
  QFont fabulist_header =
      QFont(QFontDatabase::applicationFontFamilies(font_).first(),
            static_cast<int32_t>(37 / kScale));
  QFont fabulist_general =
      QFont(QFontDatabase::applicationFontFamilies(font_).first(),
            static_cast<int32_t>(23 / kScale));
  painter->setFont(fabulist_header);
  painter->setPen(QColor(Qt::white));
  painter->drawText(width_ / 2 - width_ / 20, height_ / 7, result_);

  if (units_to_quantity_.size() > 0) {
    painter->drawText(width_ / 2 - width_ / 4, height_ / 4, caption_);
  } else {
    painter->drawText(width_ / 2 - width_ / 9, height_ / 4, "No one is dead");
  }
  uint64_t counter = 0;
  auto unit_to_quantity = units_to_quantity_.begin();
  painter->setFont(fabulist_general);
  for (int32_t y = kUnitsTextTop;
       y < kUnitsTextBottom - kStepY && counter < units_to_quantity_.size();
       y += kStepY) {
    for (int32_t x = kUnitsTextLeft;
         x < kUnitsTextRight - kStepX && counter < units_to_quantity_.size();
         x += kStepX) {
      int32_t number_of_dead = unit_to_quantity->second;
      painter->drawText(
          x, y + height_ / 5,
          ObjectsStorage::GetUnitCaption(unit_to_quantity->first));

      painter->drawText(x + 2 * kStepX / 3, y + height_ / 5,
                        QString::number(number_of_dead));
      unit_to_quantity++;
      counter++;
    }
  }
  Q_UNUSED(option)
  Q_UNUSED(widget)
}
