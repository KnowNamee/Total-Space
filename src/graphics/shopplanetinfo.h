#ifndef SHOPPLANETINFO_H
#define SHOPPLANETINFO_H

#include <QGraphicsItem>

class ShopPlanetInfo : QGraphicsItem{
    struct UnitData {
      QPixmap* unit_image;
      QString caption;
      int32_t quantity = 0;
    };

public:
    ShopPlanetInfo();

private:
    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
               QWidget* widget) override;

    QPixmap* unit_image;
    QString caption;
    int32_t quantity = 0;

    int32_t width_;
    int32_t height_;
}

#endif // SHOPPLANETINFO_H
