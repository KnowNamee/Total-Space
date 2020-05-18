#include "graphics/shopwidget.h"

#include <scene/gamescene.h>

#include <QBrush>
#include <QDebug>
#include <QFontDatabase>
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QPainter>
#include <QPen>

#include "core/menu.h"
#include "core/statemachine.h"
#include "data/loader.h"
#include "graphics/shopplanetinfo.h"
#include "objects/unit.h"

ShopWidget::ShopWidget(int32_t width, int32_t height, ShopItemType type,
                       QString name, Resources cost, ShopPlanetInfo *info)
    : type_(type),
      width_(width),
      height_(height),
      font_(Loader::GetFont()),
      object_name_(name),
      cost_(cost),
      object_info_(info) {
  // TO DO определить рамки рамки, чтобы было beautiful
  border_ = static_cast<int32_t>(width_ * 0.95);
  buy_btn_ = new ButtonItem(width_ * 2 / 3,
                            static_cast<int32_t>(height_ * kBtnCoef), false);

  buy_btn_->SetPixmap(Loader::GetButtonImage(ButtonsEnum::kBuyButton));
  if (type == ShopItemType::kBuilding) {
    BuildingType building = ObjectsStorage::GetBuildingType(name);
    unit_image_ = Loader::GetBuildingImage(building);
  }
  if (type == ShopItemType::kUnit) {
    unit_image_ = Loader::GetUnitImage(ObjectsStorage::GetUnitType(name));
  }
  CheckEnabled();
  connect(buy_btn_, SIGNAL(clicked()), object_info_, SLOT(IncQuant()));
  connect(buy_btn_, SIGNAL(clicked()), this, SLOT(Purchase()));
}

QRectF ShopWidget::boundingRect() const {
  return QRectF(-width_ / 2, -height_ / 2, width_, height_);
}

void ShopWidget::paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) {
  Q_UNUSED(option);
  Q_UNUSED(widget);

  QFont fabulist_general =
      QFont(QFontDatabase::applicationFontFamilies(font_).first(),
            static_cast<int32_t>(Controller::scene->GetFontSize(20)));
  // TODO kscale for font
  painter->setFont(fabulist_general);
  painter->setPen(QColor(Qt::white));

  if (unit_image_ != nullptr) {
    painter->drawPixmap(
        QRect(-width_ / 4, -height_ / 3, width_ / 2, height_ / 2),
        *unit_image_);
  }

  painter->drawText(
      QRectF((QPointF(-width_ / 5, -height_ / 2 + height_ / 10)),
             QSizeF(width_, height_ * kNameZoneCoef + height_ / 30)),
      object_name_.toLower());
  //
  painter->drawText(
      QRectF((QPointF(-width_ / 2 + width_ / 20,
                      height_ * (kNameZoneCoef) + height_ / 20)),
             QSizeF(width_, height_ * kInfoCoef + height_ / 20)),
      "tools: " + QString::number(cost_.GetTools()) +
          " | batteries: " + QString::number(cost_.GetBatteries()));

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

void ShopWidget::SetScene(QGraphicsScene *scene) { scene_ = scene; }

void ShopWidget::WidgetShow() {
  show();
  buy_btn_->show();
}

void ShopWidget::WidgetHide() {
  hide();
  buy_btn_->hide();
}

const QString &ShopWidget::GetName() const { return object_name_; }

const Resources &ShopWidget::GetCost() const { return cost_; }

ShopItemType ShopWidget::GetType() const { return type_; }

void ShopWidget::CheckEnabled() const {
  if (type_ == ShopItemType::kBuilding) {
    BuildingType building = ObjectsStorage::GetBuildingType(object_name_);
    if (!Controller::GetActivePlanet()->CanBuyBuilding(building)) {
      buy_btn_->setEnabled(false);
    }
  }
  if (type_ == ShopItemType::kUnit) {
    if (!Controller::GetActivePlanet()->CanBuyUnit(
            ObjectsStorage::GetUnitType(object_name_))) {
      buy_btn_->SetEnabled(false);
    }
  }
}

void ShopWidget::Purchase() {
  Controller::GetShopMenu()->MakePurchase(type_, object_name_);
  CheckEnabled();
}
