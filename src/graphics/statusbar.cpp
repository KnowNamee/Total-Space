#include "statusbar.h"

#include <QFont>
#include <QFontDatabase>
#include <QPainter>
#include <QDebug>

#include "core/statemachine.h"
#include "data/loader.h"
#include "objects/player.h"
#include "scene/gamescene.h"
#include "scene/gameview.h"

StatusBar::StatusBar(int32_t width, int32_t height)
    : width_(width), height_(height) {
  batteries_ = Loader::GetButtonImage(ButtonsEnum::kBatteriesIcon);
  tools_ = Loader::GetButtonImage(ButtonsEnum::kToolsIcon);
  army_power_ = Loader::GetButtonImage(ButtonsEnum::kArmyPowerIcon);
  setFlag(ItemIsSelectable);
}

void StatusBar::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    Q_UNUSED(event);
}

QRectF StatusBar::boundingRect() const {
  double scale = Controller::view->matrix().m11();
  double width = width_ / scale;
  double height = height_ / scale;

  return QRectF(-width / 2, -height / 2, width, height);
}

void StatusBar::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                      QWidget *widget) {
  Q_UNUSED(option);
  Q_UNUSED(widget);

  int32_t width = static_cast<int32_t>(boundingRect().width());
  int32_t height = static_cast<int32_t>(boundingRect().height());
  QSize field_size(width / kFieldsCount, height);

  QFont font =
      QFont(QFontDatabase::applicationFontFamilies(Loader::GetFont()).first(),
            static_cast<int32_t>(Controller::scene->GetFontSize(30) /
                                 Controller::view->matrix().m11()));
  painter->setFont(font);

  painter->setPen(QPen(Qt::white, 20));

  painter->drawPixmap(QRect(-width / 2 + width / 13, -height / 2,
                            field_size.width() / 2, field_size.width() / 2),
                      *tools_);
  painter->drawPixmap(
      QRect(-width / 2 + field_size.width() + width / 13, -height / 2,
            field_size.width() / 2, field_size.width() / 2),
      *batteries_);
  painter->drawPixmap(
      QRect(-width / 2 + 2 * field_size.width() + width / 13, -height / 2,
            field_size.width() / 2, field_size.width() / 2),
      *army_power_);
  painter->drawText(
      QRectF(-width / 2, -height / 3 + height / 20 + field_size.height() / 2,
             field_size.width(), field_size.height() / 2),
      Qt::AlignHCenter,
      QString::number(Controller::scene->GetPlayer()->GetTools()));

  painter->drawText(
      QRectF(-width / 2 + field_size.width(),
             -height / 3 + height / 20 + field_size.height() / 2,
             field_size.width(), field_size.height() / 2),
      Qt::AlignHCenter,
      QString::number(Controller::scene->GetPlayer()->GetBatteries()));

  painter->drawText(
      QRectF(-width / 2 + 2 * field_size.width(),
             -height / 3 + height / 20 + field_size.height() / 2,
             field_size.width(), field_size.height() / 2),
      Qt::AlignHCenter,
      QString::number(Controller::scene->GetPlayer()->GetArmyPower()));
}
