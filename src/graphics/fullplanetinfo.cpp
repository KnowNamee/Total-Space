#include "fullplanetinfo.h"

#include <QPixmap>

#include "core/statemachine.h"
#include "graphics/planetgraphics.h"
#include "objects/planet.h"
#include "objects/player.h"
#include "scene/gamescene.h"
#include "scene/gameview.h"

FullPlanetInfo::FullPlanetInfo(int32_t width, int32_t height, Planet *planet)
    : width_(width), height_(height) {
  PlanetGraphics* planet_graphics = dynamic_cast<PlanetGraphics*>(
      Controller::scene->itemAt(2 * planet->GetCoordinates(), QTransform()));
  planet_image_ = planet_graphics->GetImage();
  // TODO
  // Planet Name
  if (planet->GetOwner() != nullptr) {
    name_ = planet->GetOwner()->GetName();
  } else {
    name_ = "No Owner";
  }
  level_ = planet->GetLevel();
  Resources res = planet->GetUpgradeCost();
  tools_cost_ = res.GetTools();
  batteries_cost_ = res.GetBatteries();
  is_players_ = Controller::GetActivePlanet()->GetOwner() ==
                dynamic_cast<PlayerBase*>(Controller::scene->GetPlayer());
  nearest_power_ =
      Controller::scene->GetNearestPower(Controller::scene->GetPlayer());
  power_ = planet->GetPower();
  if (is_players_) {
    Resources income = Controller::GetActivePlanet()->GetIncome();
    tools_income_ = income.GetTools();
    batteries_income_ = income.GetBatteries();
  }
  units_to_data_ = Controller::GetActivePlanet()->GetUnitsToData();
}

void FullPlanetInfo::SetLevel(int32_t level) {
  level_ = level;
  Resources res = Controller::GetActivePlanet()->GetUpgradeCost();
  tools_cost_ = res.GetTools();
  batteries_cost_ = res.GetBatteries();
  update();
}

QRectF FullPlanetInfo::boundingRect() const {
  const double kScale = Controller::view->matrix().m11();
  return QRectF(-width_ / kScale / 2, -height_ / kScale / 2, width_ / kScale,
                height_ / kScale);
}

void FullPlanetInfo::paint(QPainter* painter,
                           const QStyleOptionGraphicsItem* option,
                           QWidget* widget) {
  // Planet
  painter->drawPixmap(QRect(static_cast<int32_t>(-boundingRect().width() / 2),
                            static_cast<int32_t>(-boundingRect().width() / 6),
                            static_cast<int32_t>(boundingRect().width() / 3),
                            static_cast<int32_t>(boundingRect().width() / 3)),
                      *planet_image_);
  painter->setBrush(QColor(Qt::white));
  painter->setPen(QColor(Qt::white));
  painter->drawText(static_cast<int32_t>(-boundingRect().width() / 3),
                    static_cast<int32_t>(-boundingRect().height() * 7 / 16),
                    "Level " + QString::number(level_));
  if (is_players_) {
    painter->drawText(static_cast<int32_t>(-boundingRect().width() / 2),
                      static_cast<int32_t>(boundingRect().height() / 2),
                      "Tools cost " + QString::number(tools_cost_));
    painter->drawText(static_cast<int32_t>(-boundingRect().width() / 4),
                      static_cast<int32_t>(boundingRect().height() / 2),
                      "Batteries cost " + QString::number(batteries_cost_));
  }

  // TODO
  // Место для лора
  painter->drawText(
      QRect(static_cast<int32_t>(-boundingRect().width() / 6),
            static_cast<int32_t>(-boundingRect().height() * 7 / 16),
            static_cast<int32_t>(boundingRect().width() / 3),
            static_cast<int32_t>(boundingRect().height() / 4)),
      "It is a dark time for the "
      "Rebellion. Although the Death "
      "Star has been destroyed, "
      "Imperial troops have driven the "
      "Rebel forces from their hidden "
      "base and pursued them across "
      "the galaxy.");

  // Owner
  painter->drawText(
      QRect(static_cast<int32_t>(-boundingRect().width() / 6),
            static_cast<int32_t>(-boundingRect().height() * 3 / 16),
            static_cast<int32_t>(boundingRect().width() / 3),
            static_cast<int32_t>(boundingRect().height() / 8)),
      "Owner: " + name_);

  // Status
  QString status;
  QColor color;
  if (is_players_) {
    status = "Belongs to you";
    color = Qt::white;
  } else if (abs(power_ - nearest_power_) < 400) {
    status = "Attackable";
    color = Qt::yellow;
  } else if (nearest_power_ > power_) {
    status = "Approachable";
    color = Qt::green;
  } else {
    status = "Inaccessible";
    color = Qt::red;
  }
  painter->setPen(color);
  painter->drawText(
      QRect(static_cast<int32_t>(-boundingRect().width() / 6),
            static_cast<int32_t>(-boundingRect().height() * 1 / 16),
            static_cast<int32_t>(boundingRect().width() / 3),
            static_cast<int32_t>(boundingRect().height() / 8)),
      "Status: " + status);

  painter->setPen(QColor(Qt::white));

  // Income
  if (is_players_) {
    painter->drawText(
        QRect(static_cast<int32_t>(-boundingRect().width() / 6),
              static_cast<int32_t>(boundingRect().height() * 1 / 16),
              static_cast<int32_t>(boundingRect().width() / 3),
              static_cast<int32_t>(boundingRect().height() / 8)),
        "Batteries Income: " + QString::number(batteries_income_));
    painter->drawText(
        QRect(static_cast<int32_t>(-boundingRect().width() / 6),
              static_cast<int32_t>(boundingRect().height() * 2 / 16),
              static_cast<int32_t>(boundingRect().width() / 3),
              static_cast<int32_t>(boundingRect().height() / 8)),
        "Tools Income: " + QString::number(tools_income_));
  }

  // Units
  painter->drawText(
      QRect(static_cast<int32_t>(boundingRect().width() * 7 / 24),
            static_cast<int32_t>(-boundingRect().height() * 7 / 16),
            static_cast<int32_t>(boundingRect().width() / 3),
            static_cast<int32_t>(boundingRect().height() / 8)),
            "Units");
  int32_t units_x = static_cast<int32_t>(boundingRect().width() / 4);
  int32_t unit_y = static_cast<int32_t>(-boundingRect().height() / 4);
  for (const auto &unit_to_data : units_to_data_) {
    //   TODO
    //   отрисовка картинки юнита подобрать размеры
    painter->drawText(units_x - width_ / 15, unit_y,
                      unit_to_data.second.caption);
    painter->drawText(units_x + width_ / 15, unit_y,
                      QString::number(unit_to_data.second.quantity));
    // TODO
    // Расстояние между юнитами
    unit_y += static_cast<int32_t>(boundingRect().height() / 8);
  }

  Q_UNUSED(option)
  Q_UNUSED(widget)
}
