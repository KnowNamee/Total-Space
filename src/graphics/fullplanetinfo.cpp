#include "fullplanetinfo.h"

#include <QFontDatabase>
#include <QPixmap>

#include "core/statemachine.h"
#include "data/loader.h"
#include "graphics/planetgraphics.h"
#include "objects/planet.h"
#include "objects/player.h"
#include "scene/gamescene.h"
#include "scene/gameview.h"

FullPlanetInfo::FullPlanetInfo(int32_t width, int32_t height, Planet* planet)
    : font_(Loader::GetFont()), width_(width), height_(height) {
  PlanetGraphics* planet_graphics = dynamic_cast<PlanetGraphics*>(
      Controller::scene->itemAt(2 * planet->GetCoordinates(), QTransform()));
  planet_image_ = planet_graphics->GetImage();
  // TODO
  // Planet Name
  if (planet->GetOwner() != nullptr) {
    name_ = planet->GetOwner()->GetName().toLower();
  } else {
    name_ = "no owner";
  }
  level_ = planet->GetLevel();
  Resources res = planet->GetUpgradeCost();
  tools_cost_ = res.GetTools();
  batteries_cost_ = res.GetBatteries();
  is_players_ = Controller::GetActivePlanet()->GetOwner() ==
                dynamic_cast<PlayerBase*>(Controller::scene->GetPlayer());
  if (!is_players_) {
    PlayerBase* player = Controller::scene->GetPlayer();
    QVector<UnitType> defending_units = planet->GetUnits();
    defending_units.append(planet->GetTiredUnits());
    result_ = planet->CalculateAttack(planet->GetNearestEnemies(player),
                                      defending_units);
  }
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
  painter->setPen(QColor(Qt::white));
  const double kScale = Controller::view->matrix().m11();
  QFont fabulist_header =
      QFont(QFontDatabase::applicationFontFamilies(font_).first(),
            static_cast<int32_t>(37 / kScale));
  painter->setFont(fabulist_header);
  QFont fabulist_general =
      QFont(QFontDatabase::applicationFontFamilies(font_).first(),
            static_cast<int32_t>(20 / kScale));
  painter->drawPixmap(
      QRect(static_cast<int32_t>(-boundingRect().width() / 2),
            static_cast<int32_t>(-boundingRect().width() / 6),
            static_cast<int32_t>(boundingRect().width() * 2 / 7),
            static_cast<int32_t>(boundingRect().width() * 2 / 7)),
      *planet_image_);
  painter->setBrush(QColor(Qt::white));
  painter->setPen(QColor(Qt::white));
  painter->drawText(static_cast<int32_t>(-boundingRect().width() / 3 -
                                         boundingRect().width() / 20),
                    static_cast<int32_t>(-boundingRect().height() * 6 / 16),
                    "level " + QString::number(level_));
  if (is_players_) {
    painter->drawText(static_cast<int32_t>(-boundingRect().width() / 2 +
                                           boundingRect().width() / 20),
                      static_cast<int32_t>(boundingRect().height() / 2),
                      "tools cost " + QString::number(tools_cost_));
    painter->drawText(static_cast<int32_t>(-boundingRect().width() / 4),
                      static_cast<int32_t>(boundingRect().height() / 2),
                      "batteries cost " + QString::number(batteries_cost_));
  }

  // TODO
  // Место для лора
  painter->setFont(fabulist_general);
  painter->drawText(
      QRect(static_cast<int32_t>(-boundingRect().width() / 6),
            static_cast<int32_t>(-boundingRect().height() * 7 / 16),
            static_cast<int32_t>(boundingRect().width() / 3),
            static_cast<int32_t>(boundingRect().height() / 4)),
      "it is a dark time for the "
      "rebellion. although the death "
      "star has been destroyed, "
      "imperial troops have driven the "
      "rebel forces from their hidden "
      "base and pursued them across "
      "the galaxy.");

  // Owner
  painter->setFont(fabulist_header);
  painter->drawText(
      QRect(static_cast<int32_t>(-boundingRect().width() / 6),
            static_cast<int32_t>(-boundingRect().height() * 3 / 16),
            static_cast<int32_t>(boundingRect().width() / 3),
            static_cast<int32_t>(boundingRect().height() / 8)),
      "owner: " + name_);

  // Status
  QString status;
  QColor color;
  if (is_players_) {
    status = "belongs to you";
    color = Qt::white;
  } else if (result_ == Planet::AttackResult::kDraw) {
    status = "attackable";
    color = Qt::yellow;
  } else if (result_ == Planet::AttackResult::kWin) {
    status = "approachable";
    color = Qt::green;
  } else {
    status = "inaccessible";
    color = Qt::red;
  }
  painter->setPen(color);
  painter->drawText(
      QRect(static_cast<int32_t>(-boundingRect().width() / 6),
            static_cast<int32_t>(-boundingRect().height() * 1 / 16),
            static_cast<int32_t>(boundingRect().width() / 3),
            static_cast<int32_t>(boundingRect().height() / 8)),
      "status: " + status);

  painter->setPen(QColor(Qt::white));

  // Income
  if (is_players_) {
    painter->drawText(
        QRect(static_cast<int32_t>(-boundingRect().width() / 6),
              static_cast<int32_t>(boundingRect().height() * 1 / 16),
              static_cast<int32_t>(boundingRect().width() / 3),
              static_cast<int32_t>(boundingRect().height() / 8)),
        "batteries income: " + QString::number(batteries_income_));
    painter->drawText(
        QRect(static_cast<int32_t>(-boundingRect().width() / 6),
              static_cast<int32_t>(boundingRect().height() * 3 / 16),
              static_cast<int32_t>(boundingRect().width() / 3),
              static_cast<int32_t>(boundingRect().height() / 8)),
        "tools income: " + QString::number(tools_income_));
  }

  // Units
  painter->drawText(
      QRect(static_cast<int32_t>(boundingRect().width() * 7 / 24),
            static_cast<int32_t>(-boundingRect().height() * 7 / 16),
            static_cast<int32_t>(boundingRect().width() / 3),
            static_cast<int32_t>(boundingRect().height() / 8)),
      "units");
  int32_t units_x = static_cast<int32_t>(boundingRect().width() / 4);
  int32_t unit_y = static_cast<int32_t>(-boundingRect().height() / 4);
  for (const auto& unit_to_data : units_to_data_) {
    painter->drawText(units_x - units_x / 4, unit_y,
                      unit_to_data.second.caption.toLower());
    painter->drawText(units_x + units_x / 4, unit_y,
                      QString::number(unit_to_data.second.quantity).toLower());
    if (unit_to_data.second.unit_image != nullptr) {
      painter->drawPixmap(
          QRect(units_x + units_x / 2, unit_y - units_x / 8,
                static_cast<int32_t>(boundingRect().width() / 20),
                static_cast<int32_t>(boundingRect().width() / 20)),
          *unit_to_data.second.unit_image);
    }
    unit_y += static_cast<int32_t>(boundingRect().height() / 8);
  }

  Q_UNUSED(option)
  Q_UNUSED(widget)
}
