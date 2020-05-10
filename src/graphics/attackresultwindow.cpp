#include "attackresultwindow.h"

#include <QPainter>

#include "data/objectsstorage.h"

AttackResultWindow::AttackResultWindow(
    const std::map<UnitType, int32_t>& units_to_quantity, const QString& result,
    const QString& caption, int32_t width, int32_t height)
    : units_to_quantity_(units_to_quantity),
      result_(result),
      caption_(caption),
      width_(width),
      height_(height) {}

QRectF AttackResultWindow::boundingRect() const {
  return QRectF(0, 0, width_, height_);
}

void AttackResultWindow::paint(QPainter* painter,
                               const QStyleOptionGraphicsItem* option,
                               QWidget* widget) {
  painter->setBrush(QColor(Qt::black));
  painter->setPen(QColor(Qt::white));
  painter->drawRect(boundingRect());
  // TODO
  // Подобрать размеры текста и расположение, в зависимости от скейла
  painter->drawText(width_ / 2, height_ / 10, result_);
  if (units_to_quantity_.size() > 0) {
    painter->drawText(width_ / 2 - width_ / 20, height_ / 8, caption_);
  } else {
    painter->drawText(width_ / 2 - width_ / 20, height_ / 8, "No one is dead");
  }
  uint64_t counter = 0;
  auto unit_to_quantity = units_to_quantity_.begin();
  for (int32_t y = kUnitsTextTop;
       y < kUnitsTextBottom - kStepY && counter < units_to_quantity_.size();
       y += kStepY) {
    for (int32_t x = kUnitsTextLeft;
         x < kUnitsTextRight - kStepX && counter < units_to_quantity_.size();
         x += kStepX) {
      int32_t number_of_dead = unit_to_quantity->second;
      painter->drawText(
          x, y, ObjectsStorage::GetUnitCaption(unit_to_quantity->first));
      painter->drawText(x + 2 * kStepX / 3, y, QString::number(number_of_dead));
      unit_to_quantity++;
      counter++;
    }
  }
  Q_UNUSED(option)
  Q_UNUSED(widget)
}
