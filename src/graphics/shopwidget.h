#ifndef SHOPWIDGET_H
#define SHOPWIDGET_H

#include <QGraphicsItem>
#include "objects/unit.h"
#include "graphics/buttonitem.h"
#include "util/utility.h"

class ShopWidget : public QObject, public QGraphicsItem{
  Q_OBJECT
  Q_INTERFACES(QGraphicsItem)
public:
  ShopWidget(int32_t width, int32_t height, QString name, Resources cost);

  void Destroy();

private:
  QRectF boundingRect() const override;
  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
             QWidget* widget) override;

  int32_t width_;
  int32_t height_;
  int32_t border_;

  ButtonItem* buy_btn_ = nullptr;

  QString object_name_;
  Resources cost_;

  const double kNameZoneCoef = 0.15;
  const double kPictureCoef = 0.6;
  const double kInfoCoef = 0.15;
  const double kBtnCoef = 0.1;
};

#endif // SHOPWIDGET_H
