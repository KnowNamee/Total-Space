#ifndef BUTTONITEM_H
#define BUTTONITEM_H

#include <QGraphicsItem>

class ButtonItem : public QObject, public QGraphicsItem {
  Q_OBJECT
  Q_INTERFACES(QGraphicsItem)
 public:
  ButtonItem(int32_t width, int32_t height);
  void SetPixmap(QPixmap* button_image);

 signals:
  void clicked();

 protected:
  void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

  int32_t width_;
  int32_t height_;

 private:
  QRectF boundingRect() const override;
  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
             QWidget* widget) override;

  QPixmap* button_image_;
};

#endif  // BUTTONITEM_H
