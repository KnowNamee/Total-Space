#include "buttonitem.h"

#include <QPainter>
#include <QDebug>

ButtonItem::ButtonItem(int32_t width, int32_t height)
    : QGraphicsItem(), width_(width), height_(height) {}

QRectF ButtonItem::boundingRect() const {
  return QRectF(0, 0, width_, height_);
}

void ButtonItem::paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) {
  Q_UNUSED(widget)
  Q_UNUSED(option)
  painter->setBrush(QColor(Qt::white));
  painter->drawRect(boundingRect());
}

void ButtonItem::MouseClicked() {
  emit clicked();
}
