#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <scene/gamescene.h>
#include <QPen>
#include <QBrush>
#include "core/statemachine.h"
#include "graphics/shopwidget.h"
#include "objects/unit.h"

ShopWidget::ShopWidget(int32_t width, int32_t height, Unit* unit) : width_(width), height_(height), unit_(unit){
    // TO DO определить рамки рамки, чтобы было beautiful
    border_ = static_cast<int32_t>(width_ * 0.95);
    background_rect_ = new QGraphicsRectItem();
}

QRectF ShopWidget::boundingRect() const {
    return QRectF(-width_ / 2, -height_ / 2, width_, height_);
}

void ShopWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QRectF background = boundingRect();
    background_rect_->setRect(background);
    background_rect_->setBrush(Qt::gray);
    background_rect_->setPen(Qt::NoPen);

    Controller::scene->addItem(background_rect_);
}
