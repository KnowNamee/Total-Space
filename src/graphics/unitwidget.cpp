#include "unitwidget.h"

#include <QDebug>
#include <QFontDatabase>
#include <QGraphicsProxyWidget>
#include <QPainter>
#include <QTextItem>

#include "core/menu.h"
#include "core/statemachine.h"
#include "data/loader.h"
#include "data/objectsstorage.h"
#include "scene/gamescene.h"
#include "util/utility.h"

UnitWidget::UnitWidget(UnitsInteractionMenu* parent, Planet* planet,
                       UnitType unit, int32_t width, int32_t height)
    : ButtonItem(width, height),
      attack_(QString::number(
          ObjectsStorage::GetUnitCharacteristics(unit).GetAttack())),
      armor_(QString::number(
          ObjectsStorage::GetUnitCharacteristics(unit).GetArmor())),
      health_(QString::number(
          ObjectsStorage::GetUnitCharacteristics(unit).GetHealth())),
      stamina_(QString::number(
          ObjectsStorage::GetUnitCharacteristics(unit).GetStamina())),
      cell_unit_(unit),
      caption_(ObjectsStorage::GetUnitCaption(cell_unit_).toLower()),

      parent_(parent),
      unit_planet_(planet),
      active_widget_(Loader::GetButtonImage(ButtonsEnum::kActiveWidget)),
      unactive_widget_(Loader::GetButtonImage(ButtonsEnum::kUnactiveWidget)),
      unit_icon_(Loader::GetUnitImage(unit)),
      font_(Loader::GetFont()) {}

QRectF UnitWidget::boundingRect() const {
  return QRectF(pos().x(), pos().y(), width_, height_);
}

void UnitWidget::paint(QPainter* painter,
                       const QStyleOptionGraphicsItem* option,
                       QWidget* widget) {
  Q_UNUSED(option);
  Q_UNUSED(widget);
  QRect rect = boundingRect().toAlignedRect();
  painter->drawPixmap(rect, *current_widget_);

  int32_t unit_image_x = static_cast<int32_t>(pos().x() + width_ / 10);
  int32_t unit_image_y = static_cast<int32_t>(pos().y() + height_ / 8);

  if (unit_icon_ != nullptr) {
    painter->drawPixmap(
        QRect(unit_image_x, unit_image_y, width_ / 4, width_ / 4), *unit_icon_);
  }

  QFont fabulist_header =
      QFont(QFontDatabase::applicationFontFamilies(font_).first(),
            Controller::scene->GetFontSize(18));
  QFont fabulist_general =
      QFont(QFontDatabase::applicationFontFamilies(font_).first(),
            Controller::scene->GetFontSize(15));
  painter->setFont(fabulist_header);
  painter->setPen(QColor(Qt::white));
  painter->drawText(unit_image_x + width_ / 20,
                    static_cast<int32_t>(pos().y() + height_ * 0.8), caption_);

  painter->setFont(fabulist_general);
  painter->drawText(static_cast<int32_t>(pos().x() + width_ / 2),
                    static_cast<int32_t>(pos().y() + height_ * 0.2),
                    "health:\t" + health_);
  painter->drawText(static_cast<int32_t>(pos().x() + width_ / 2),
                    static_cast<int32_t>(pos().y() + height_ * 0.4),
                    "attack:\t" + attack_);
  painter->drawText(static_cast<int32_t>(pos().x() + width_ / 2),
                    static_cast<int32_t>(pos().y() + height_ * 0.6),
                    "armor:\t" + armor_);
  painter->drawText(static_cast<int32_t>(pos().x() + width_ / 2),
                    static_cast<int32_t>(pos().y() + height_ * 0.8),
                    "stamina:\t" + stamina_);
}

void UnitWidget::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
  SwitchWidget();
  if (is_chosen_) {
    parent_->RemoveUnit(this);
  } else {
    parent_->ChooseUnit(this);
  }
  is_chosen_ = !is_chosen_;

  Q_UNUSED(event)
}

Planet* UnitWidget::GetPlanet() const { return unit_planet_; }

UnitType UnitWidget::GetUnit() const { return cell_unit_; }

void UnitWidget::SwitchWidget() {
  if (current_widget_ == unactive_widget_) {
    current_widget_ = active_widget_;
  } else {
    current_widget_ = unactive_widget_;
  }
  update();
}
