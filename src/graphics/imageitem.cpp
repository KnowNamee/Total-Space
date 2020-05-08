#include "graphics/imageitem.h"

#include <QApplication>
#include <QDebug>
#include <QPainter>
#include <QScreen>

#include "scene/gameview.h"
#include "data/loader.h"
#include "core/statemachine.h"

ImageItem::ImageItem(const QPixmap* image, int width, int height)
    : image_(image), size_(QSize(width, height)) {}

int32_t ImageItem::type() const { return Type; }

QRectF ImageItem::boundingRect() const {
  return QRectF(pos() - QPoint(size_.width() / 2, size_.height() / 2), size_);
}

void ImageItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                      QWidget* widget) {
  QRectF rect = boundingRect();
  painter->drawPixmap(rect, *image_,
                      QRectF(0, 0, image_->width(), image_->height()));

  Q_UNUSED(option)
  Q_UNUSED(widget)
}
