#include "graphics/imageitem.h"

#include <QApplication>
#include <QDebug>
#include <QPainter>
#include <QScreen>

#include "core/statemachine.h"
#include "data/loader.h"
#include "scene/gameview.h"

ImageItem::ImageItem(const QPixmap* image, int width, int height)
    : image_(image), size_(QSize(width, height)) {}

ImageItem::ImageItem(const QPixmap* image_, QRectF background_rect)
  : image_(image_), background_rect_(background_rect) {}

int32_t ImageItem::type() const { return Type; }

QRectF ImageItem::boundingRect() const {
  if (!background_rect_.isNull()) {
    return background_rect_;
  }
  return QRectF(pos() - QPoint(size_.width() / 2, size_.height() / 2), size_);
}

void ImageItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                      QWidget* widget) {
  QRect rect = boundingRect().toAlignedRect();
  painter->drawPixmap(rect, *image_);

  Q_UNUSED(option)
  Q_UNUSED(widget)
}
