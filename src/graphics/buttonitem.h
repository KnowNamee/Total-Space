#ifndef BUTTONITEM_H
#define BUTTONITEM_H

#include <QGraphicsItem>

class ButtonItem : public QObject, public QGraphicsItem {
  Q_OBJECT
  Q_INTERFACES(QGraphicsItem)
 public:
  ButtonItem(int32_t width, int32_t height);
  virtual void MouseClicked();

 signals:
  void clicked();

 protected:
  int32_t width_;
  int32_t height_;

 private:
  QRectF boundingRect() const override;
  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
             QWidget* widget) override;
};

#endif  // BUTTONITEM_H
