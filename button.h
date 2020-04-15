#ifndef BUTTON_H
#define BUTTON_H

#include <QGraphicsItem>

#include "typeoffset.h"

class Button : public QGraphicsItem {
  Q_INTERFACES()

 public:
  Button();
  Button(const QImage &img);
  Button(const QString &str_);

  int type() const override;

 private:
  QImage img_;
  QString str_;

  QRectF boundingRect() const override;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
             QWidget *widget) override;

 public:
  enum {
    Type = UserType + TypeOffset::Button,
  };
};

#endif  // BUTTON_H
