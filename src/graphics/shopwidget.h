#ifndef SHOPWIDGET_H
#define SHOPWIDGET_H

#include <QGraphicsItem>
#include "objects/unit.h"
#include "graphics/buttonitem.h"

class ShopWidget : public QObject, public QGraphicsItem{
  Q_OBJECT
  Q_INTERFACES(QGraphicsItem)
public:
  ShopWidget(int32_t width, int32_t height, Unit* unit);

private:
  QRectF boundingRect() const override;
  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
             QWidget* widget) override;

  int32_t width_;
  int32_t height_;
  int32_t border_;

  Unit* unit_ = nullptr;
  ButtonItem* but_btn_ = nullptr;
  QGraphicsRectItem* background_rect_ = nullptr;
};

#endif // SHOPWIDGET_H
