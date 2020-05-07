#include "buttonitem.h"

#include <QDebug>
#include <QPainter>

ButtonItem::ButtonItem(int32_t width, int32_t height)
    : QGraphicsItem(), width_(width), height_(height) {
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
  return QRectF(0, 0, width_, height_);
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
