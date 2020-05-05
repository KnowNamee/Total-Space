#include "unitwidget.h"

#include <QDebug>
#include <QGraphicsProxyWidget>
#include <QPainter>
#include <QTextItem>

#include "core/menu.h"
#include "core/statemachine.h"
#include "data/objectsstorage.h"
#include "scene/gamescene.h"

UnitWidget::UnitWidget(Planet *planet, UnitType unit, int32_t width,
                       int32_t height)
    : ButtonItem(width, height),
      unit_planet_(planet),
      cell_unit_(unit),
      caption_(ObjectsStorage::GetUnitCaption(cell_unit_)) {}

QRectF UnitWidget::boundingRect() const {
  return QRectF(pos().x(), pos().y(), width_, height_);
}

void UnitWidget::paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) {
  Q_UNUSED(option);
  Q_UNUSED(widget);
  painter->setBrush(current_color_);
  painter->setPen(QColor(Qt::white));
  painter->drawRect(boundingRect());
  painter->drawText(static_cast<int32_t>(pos().x() + width_ / 10),
                    static_cast<int32_t>(pos().y() + height_ * 0.9),
                    caption_);
  painter->drawText(static_cast<int32_t>(pos().x() + width_ / 10),
                    static_cast<int32_t>(pos().y() + height_ * 0.7),
                    caption_);
  painter->drawText(static_cast<int32_t>(pos().x() + width_ / 10),
                    static_cast<int32_t>(pos().y() + height_ * 0.5),
                    caption_);
  painter->drawText(static_cast<int32_t>(pos().x() + width_ / 10),
                    static_cast<int32_t>(pos().y() + height_ * 0.3),
                    caption_);
}

void UnitWidget::MouseClicked() {
  SwitchColor();
  if (is_chosen_) {
    Controller::GetAttackMenu()->RemoveUnit(this);
  } else {
    Controller::GetAttackMenu()->ChooseUnit(this);
  }
  is_chosen_ = !is_chosen_;
}

Planet *UnitWidget::GetPlanet() const { return unit_planet_; }

UnitType UnitWidget::GetUnit() const { return cell_unit_; }

void UnitWidget::SwitchColor() {
  // Если менять цвета здесь, то и значение по умолчанию в .h файле
  if (current_color_ == Qt::black) {
    current_color_ = Qt::green;
  } else {
    current_color_ = Qt::black;
  }
  update();
}
