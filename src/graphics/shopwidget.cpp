#include "graphics/shopwidget.h"
#include "core/statemachine.h"
#include "core/menu.h"
#include "objects/unit.h"
#include <QBrush>
#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QPainter>
#include <QPen>
#include <scene/gamescene.h>

ShopWidget::ShopWidget(int32_t width, int32_t height, ShopItemType type, QString name, Resources cost)
    : width_(width), height_(height), type_(type), object_name_(name), cost_(cost){
    // TO DO определить рамки рамки, чтобы было beautiful
    border_ = static_cast<int32_t>(width_ * 0.95);
    buy_btn_ = new ButtonItem(width_,
        static_cast<int32_t>(height_ * kBtnCoef),
                              false);

    connect(buy_btn_, SIGNAL(clicked()), this, SLOT(Purchase()));
}

QRectF ShopWidget::boundingRect() const {
    return QRectF(-width_ / 2, -height_ / 2, width_, height_);
}

    void ShopWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setBrush(Qt::gray);
    painter->drawRect(boundingRect());

    painter->drawText(QRectF((QPointF(-width_ / 2, -height_ / 2)), QSizeF(width_, height_ * kNameZoneCoef)), object_name_);
    painter->drawText(QRectF((QPointF(-width_ / 2, height_ * (kNameZoneCoef + kPictureCoef - 0.5))), QSizeF(width_, height_ * kInfoCoef)),
                      "Tools: " + QString::number(cost_.GetTools()) + " | Batteries: " + QString::number(cost_.GetBatteries()));

    buy_btn_->setPos(pos() + QPointF(0, height_ * (1 - kBtnCoef) / 2));
    buy_btn_->setZValue(zValue());
    // чтобы не было ошибок повторного добавления кнопки на сцену
    if (!btn_added) {
        btn_added = true;
        scene_->addItem(buy_btn_);
    }
}

void ShopWidget::Destroy() {
    Controller::scene->removeItem(this);
    Controller::scene->removeItem(buy_btn_);

    delete buy_btn_;
    delete this;
}

void ShopWidget::SetScene(QGraphicsScene *scene) { scene_ = scene;}

void ShopWidget::WidgetShow() {
    show();
    buy_btn_->show();
}

void ShopWidget::WidgetHide() {
    hide();
    buy_btn_->hide();
}

const QString& ShopWidget::GetName() const {
    return object_name_;
}

const Resources& ShopWidget::GetCost() const {
    return cost_;
}

ShopItemType ShopWidget::GetType() const {
    return type_;
}

void ShopWidget::Purchase() {
    Controller::GetShopMenu()->MakePurchase(type_, cost_, object_name_);
}
