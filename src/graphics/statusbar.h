#ifndef STATUSBAR_H
#define STATUSBAR_H

#include <QGraphicsItem>

class StatusBar : public QObject, public QGraphicsItem {
  Q_OBJECT
  Q_INTERFACES(QGraphicsItem)

 public:
  StatusBar(int32_t width, int32_t height);

protected:
  void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

private:
  QRectF boundingRect() const override;
  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
             QWidget* widget) override;

  int32_t width_;
  int32_t height_;
  QPixmap* tools_ = nullptr;
  QPixmap* army_power_ = nullptr;
  QPixmap* batteries_ = nullptr;

  const int32_t kFieldsCount = 3;
};

#endif  // STATUSBAR_H
