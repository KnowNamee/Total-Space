#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <scene/gamescene.h>
#include <QPen>
#include <QBrush>
#include <QPainter>
#include "core/statemachine.h"
#include "graphics/shopwidget.h"
#include "objects/unit.h"

ShopWidget::ShopWidget(int32_t width, int32_t height, QString name, Resources cost)
    : width_(width), height_(height), object_name_(name), cost_(cost){
    // TO DO определить рамки рамки, чтобы было beautiful
    border_ = static_cast<int32_t>(width_ * 0.95);
    buy_btn_ = new ButtonItem(width_,
        static_cast<int32_t>(height_ * kBtnCoef),
                              false);
}

QRectF ShopWidget::boundingRect() const {
    return QRectF(-width_ / 2, -height_ / 2, width_, height_);
}

void ShopWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setBrush(Qt::gray);
    painter->drawRect(boundingRect());

    painter->drawText(QRectF(pos() - QPointF(width_ / 2, height_ / 2), QSizeF(width_, height_ * kNameZoneCoef)), object_name_);
    painter->drawText(QRectF(pos() - QPointF(width_ / 2, height_ / 2 - height_ * (kNameZoneCoef + kPictureCoef)), QSizeF(width_, height_ * kInfoCoef)),
                      "Tools: " + QString::number(cost_.GetTools()) + " | Batteries: " + QString::number(cost_.GetBatteries()));

    buy_btn_->setPos(pos() + QPointF(0, height_ * (1 - kBtnCoef) / 2));
    buy_btn_->setZValue(zValue());
    Controller::scene->addItem(buy_btn_);

}

void ShopWidget::Destroy() {
    Controller::scene->removeItem(this);
    Controller::scene->removeItem(buy_btn_);

    delete buy_btn_;
    delete this;
}
