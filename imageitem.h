#ifndef IMAGEITEM_H
#define IMAGEITEM_H

#include <QGraphicsItem>

#include "memory"
#include "util/typeoffset.h"

class ImageItem : public QGraphicsItem {
  Q_INTERFACES()

 public:
  ImageItem() = default;
  ImageItem(const QPixmap *image_, int width, int heigt);

  int type() const override;

 private:
  const QPixmap *image_;
  QSize size_;

  QRectF boundingRect() const override;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
             QWidget *widget) override;

 public:
  enum {
    Type = UserType + TypeOffset::Button,
  };
};

#endif  // IMAGEITEM_H
