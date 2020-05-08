#include "buttonitem.h"

#include <QDebug>
#include <QPainter>

#include "core/statemachine.h"
#include "scene/gameview.h"

ButtonItem::ButtonItem(int32_t width, int32_t height, bool is_scalable)
    : QGraphicsItem(),
      width_(width),
      height_(height),
      is_scalable_(is_scalable) {
  setFlag(ItemIsSelectable);
}

void ButtonItem::SetPixmap(QPixmap* button_image) {
  button_image_ = button_image;
}

void ButtonItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
  emit clicked();

  Q_UNUSED(event);
}

QRectF ButtonItem::boundingRect() const {
  double scale = Controller::view->matrix().m11();
  if (!is_scalable_) {
    scale = 1;
  }
  double width = width_ / scale;
  double height = height_ / scale;
  return QRectF(-width / 2, -height / 2, width, height);
}

void ButtonItem::paint(QPainter* painter,
                       const QStyleOptionGraphicsItem* option,
                       QWidget* widget) {
  Q_UNUSED(widget)
  Q_UNUSED(option)
  painter->setBrush(QColor(Qt::white));
  // TODO
  // Отрисовка кнопки
  painter->drawRect(boundingRect());
}
int ButtonItem::type() const { return Type; }
