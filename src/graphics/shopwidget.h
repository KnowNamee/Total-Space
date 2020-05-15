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
  ShopWidget(int32_t width, int32_t height, ShopItemType type, QString name, Resources cost);

  void Destroy();
  void SetScene(QGraphicsScene* scene);

  const QString& GetName() const;
  const Resources& GetCost() const;
  ShopItemType GetType() const;

  void WidgetShow();
  void WidgetHide();

private slots:
  void Purchase();

private:
  ShopItemType type_;
  QRectF boundingRect() const override;
  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
             QWidget* widget) override;

  int32_t width_;
  int32_t height_;
  int32_t border_;

  QGraphicsScene* scene_ = nullptr;
  bool btn_added = false;
  ButtonItem* buy_btn_ = nullptr;

  QString object_name_;
  Resources cost_;

  const double kNameZoneCoef = 0.075;
  const double kPictureCoef = 0.65;
  const double kInfoCoef = 0.075;
  const double kBtnCoef = 0.2;
};

#endif // SHOPWIDGET_H
