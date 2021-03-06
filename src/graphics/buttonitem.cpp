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

void ButtonItem::SetEnabled(bool is_enabled) { is_enabled_ = is_enabled; }

void ButtonItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
  if (is_enabled_) {
    emit clicked();
    QMediaPlayer* click_sound = Loader::GetClickSound();
    click_sound->setVolume(20);
    click_sound->play();
  }

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

QPainterPath ButtonItem::shape() const {
  QPainterPath path;
  path.addRect(boundingRect());
  return path;
}

void ButtonItem::paint(QPainter* painter,
                       const QStyleOptionGraphicsItem* option,
                       QWidget* widget) {
  Q_UNUSED(widget)
  Q_UNUSED(option)
  if (button_image_ == nullptr) {
    painter->setBrush(QColor(Qt::white));
    painter->drawRect(boundingRect());
  } else {
    QRect rect = boundingRect().toAlignedRect();
    painter->drawPixmap(rect, *button_image_);
  }
}
int ButtonItem::type() const { return Type; }
