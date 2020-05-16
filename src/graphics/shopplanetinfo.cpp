#include "graphics/shopplanetinfo.h"
#include <QPainter>

ShopPlanetInfo::ShopPlanetInfo(int32_t width, int32_t height, QString caption,
                               int32_t quantity, QPixmap* image) :
    width_(width), height_(height), item_image_(image),
    caption_(caption), quantity_(quantity) {
    if (quantity_ == 0) {
        hide();
    }
}

const int32_t& ShopPlanetInfo::GetQuant() { return quantity_;}

void ShopPlanetInfo::IncQuant() {
    ++quantity_;
    show();
}

QRectF ShopPlanetInfo::boundingRect() const {
    return QRectF(-width_ / 2, -height_ / 2, width_, height_);
}

void ShopPlanetInfo::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);

    //TO DO рисовать картинку
    painter->setBrush(Qt::green);
    painter->drawRect(QRectF(- width_ / 2, - height_ / 2, width_ / 2, height_));

    painter->drawText(QRectF(0, -height_ / 2, width_ / 2, height_ / 2), caption_);
    painter->drawText(QRectF(0, 0, width_ / 2, height_ / 2), QString::number(quantity_));
}
