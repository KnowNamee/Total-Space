#ifndef BUTTON_H
#define BUTTON_H

#include <QGraphicsItem>

#include "memory"
#include "typeoffset.h"

class ImageItem : public QGraphicsItem {
  Q_INTERFACES()

 public:
  ImageItem() = default;
  ImageItem(const QPixmap &image_, int width, int heigt);

  int type() const override;

 private:
  QPixmap image_;
  QSize size_;

  QRectF boundingRect() const override;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
             QWidget *widget) override;

 public:
  enum {
    Type = UserType + TypeOffset::Button,
  };
};

#endif  // BUTTON_H
