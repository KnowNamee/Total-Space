#ifndef BUTTONITEM_H
#define BUTTONITEM_H

#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>

#include "util/typeoffset.h"

class ButtonItem : public QObject, public QGraphicsItem {
  Q_OBJECT
  Q_INTERFACES(QGraphicsItem)
 public:
  ButtonItem(int32_t width, int32_t height);
  virtual void MouseClicked();

  int type() const override;

  enum {
    Type = UserType + TypeOffset::kButtonItem,
  };

 signals:
  void clicked();

 protected:
  int32_t width_;
  int32_t height_;

 private:
  QRectF boundingRect() const override;
  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
             QWidget* widget) override;

 protected:
  virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* mouseEvent) override;
};

#endif  // BUTTONITEM_H
