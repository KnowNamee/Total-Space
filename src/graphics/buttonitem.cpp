#include "buttonitem.h"

#include <QDebug>
#include <QPainter>

#include "core/statemachine.h"
#include "scene/gameview.h"

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
  const double kScale = Controller::view->matrix().m11();
  double width = width_ / kScale;
  double height = height_ / kScale;
  return QRectF(-width / 2, -height / 2, width , height);
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
