#include "core/eventhandling.h"

#include <QApplication>
#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QObject>
#include <QScreen>
#include <QScrollBar>
#include <QTextCodec>
#include <QTimer>
#include <cmath>
#include <thread>

#include "core/menu.h"
#include "core/planetsgraph.h"
#include "core/statemachine.h"
#include "graphics/imageitem.h"
#include "graphics/planetgraphics.h"
#include "mainwindow.h"
#include "objects/planet.h"
#include "objects/player.h"
#include "scene/gamescene.h"
#include "scene/gameview.h"

EventHandler::View::View(GameView* view)
    : view_(view), kMapSize(view_->GetScene()->GetMapSize()) {
  timer_ = nullptr;
}

bool EventHandler::View::CompareMotion(
    EventHandler::View::MotionType needed_motion) {
  return current_motion_ == needed_motion;
}

bool EventHandler::View::IsMouseInMotionZone(QPointF cursor) {
  // TODO
  // Тоже нужно выбрать область, в которой будет двигаться экран
  double width = view_->rect().width();
  double height = view_->rect().height();
  return cursor.x() > width - width / kMoveZone ||
         cursor.y() > height - width / kMoveZone ||
         cursor.x() < width / kMoveZone || cursor.y() < width / kMoveZone;
}

void EventHandler::View::MouseMoveEvent() {
  if (Controller::GetMenuType() == Controller::MenuType::kGame &&
      current_motion_ != MotionType::kBotsAttack) {
    if (IsMouseInMotionZone(QCursor::pos())) {
      if (CompareMotion(MotionType::kMoveWithMouse)) {
        return;
      }
      if (timer_ == nullptr) {
        current_motion_ = MotionType::kMoveWithMouse;
        timer_ = new QTimer();
        timer_->start(15);
        connect(timer_, SIGNAL(timeout()), this, SLOT(Move()));
      }
    } else if (CompareMotion(MotionType::kMoveWithMouse)) {
      delete timer_;
      timer_ = nullptr;
      current_motion_ = MotionType::kNoMotion;
    }
  }
}

void EventHandler::View::MouseReleaseEvent(QMouseEvent* event) {
  Controller::MenuType state = Controller::GetMenuType();
  QGraphicsItem* item =
      view_->scene()->itemAt(view_->mapToScene(event->pos()), QTransform());

  if (item == nullptr) {
    if (state == Controller::MenuType::kPlanet) {
      Controller::SwitchMenu(Controller::MenuType::kGame);
    }
    return;
  }

  if (state == Controller::MenuType::kPlanet) {
    if (item->type() == PlanetGraphics::Type) {
      Planet* planet = dynamic_cast<PlanetGraphics*>(item)->GetPlanet();
      if (planet != Controller::GetActivePlanet()) {
        Controller::SwitchMenu(Controller::MenuType::kGame);
      }
    }
  } else if (state == Controller::MenuType::kGame) {
    if (current_motion_ == MotionType::kNoMotion) {
      Controller::SetActivePlanet(nullptr);
      Controller::scene->UpdatePlanetsGraph();
    }
  }
}

void EventHandler::View::Move() {
  Controller::MenuType state = Controller::GetMenuType();
  if (state != Controller::MenuType::kGame) {
    current_motion_ = MotionType::kNoMotion;
    if (timer_) {
      delete (timer_);
    }
    timer_ = nullptr;
    return;
  }

  int32_t width = view_->rect().width();
  int32_t height = view_->rect().height();
  QPointF cursor = QCursor::pos();
  // TODO
  // Тоже нужно выбрать область, в которой будет двигаться экран
  if (IsMouseInMotionZone(cursor)) {
    double x_direction = cursor.x() - width / 2;
    double y_direction = cursor.y() - height / 2;

    double scale_coeff_x =
        1. * x_direction /
        (sqrt(x_direction * x_direction + y_direction * y_direction));

    // TODO
    // Выбрать скорость перемещения
    double velocity =
        width / view_->matrix().m11() / 60;  // 20px на моем экране

    double x_velocity = velocity * scale_coeff_x;
    double y_velocity = sqrt(velocity * velocity - x_velocity * x_velocity);
    if (y_direction < 0) {
      y_velocity *= -1;
    }

    // TODO
    // Размеры карты тоже выбрать надо
    const double kScaleCoefficient = view_->matrix().m11() - 1 / 3;
    const double kSizeCoeff = 0.7;
    if ((view_->sceneRect().x() >=
             (kScaleCoefficient + kSizeCoeff * kMapSize) * width &&
         x_velocity > 0) ||
        (view_->sceneRect().x() <=
             -((kScaleCoefficient + kSizeCoeff * kMapSize) * width) &&
         x_velocity < 0)) {
      x_velocity = 0;
    }

    if ((view_->sceneRect().y() >=
             (kScaleCoefficient + kSizeCoeff * kMapSize) * height &&
         y_velocity > 0) ||
        (view_->sceneRect().y() <=
             -((kScaleCoefficient + kSizeCoeff * kMapSize) * height) &&
         y_velocity < 0)) {
      y_velocity = 0;
    }

    view_->setSceneRect(view_->sceneRect().x() + x_velocity,
                        view_->sceneRect().y() + y_velocity, width, height);
    Controller::GetGameMenu()->ReDraw();
  }
}

void EventHandler::View::DoubleClick(QMouseEvent* event) {
  if (Controller::GetMenuType() == Controller::MenuType::kGame &&
      current_motion_ != MotionType::kBotsAttack) {
    QGraphicsItem* item =
        view_->scene()->itemAt(view_->mapToScene(event->pos()), QTransform());
    if (item != nullptr && timer_ == nullptr &&
        item->type() == PlanetGraphics::Type) {
      double scale = view_->matrix().m11();

      QPointF event_pos = scale * view_->mapToScene(event->pos());

      double distance = (event_pos.x() - 2 * scale * item->pos().x()) *
                            (event_pos.x() - 2 * scale * item->pos().x()) +
                        (event_pos.y() - 2 * scale * item->pos().y()) *
                            (event_pos.y() - 2 * scale * item->pos().y());
      if (distance > scale * scale * (item->boundingRect().height() / 2) *
                         (item->boundingRect().height() / 2)) {
        return;
      }
      current_motion_ = MotionType::kMoveToPlanet;
      target_ = item;
      target_position_ = target_->pos();
      timer_ = new QTimer();
      timer_->start(15);
      is_scaled_motion = true;
      connect(timer_, SIGNAL(timeout()), this, SLOT(MoveTo()));
    }
  }
}

void EventHandler::View::KeyReleaseEvent(QKeyEvent* event) {
  if (current_motion_ == MotionType::kBotsAttack) {
    return;
  }
  Controller::MenuType state = Controller::GetMenuType();
  QShortcut* shortcut;
  Qt::Key unused_key = Qt::Key_ToggleCallHangup;  // Фиктивная кнопка

  if (state == Controller::MenuType::kLoad) {
    return;
  }

  // TODO lambda-function to SafeShortcutActivate
  if (state == Controller::MenuType::kPlanet) {
    shortcut = Controller::GetPlanetMenu()->GetShortcut(event->key());
    if (shortcut && shortcut->key() != QKeySequence(unused_key)) {
      shortcut->setObjectName("From KeyReleaseEvent");
      emit shortcut->activated();
    }
  } else if (state == Controller::MenuType::kGame) {
    shortcut = Controller::GetGameMenu()->GetShortcut(event->key());
    if (shortcut && shortcut->key() != QKeySequence(unused_key)) {
      shortcut->setObjectName("From KeyReleaseEvent");
      emit shortcut->activated();
    }
  } else if (state == Controller::MenuType::kPlanetInfo) {
    shortcut = Controller::GetPlanetInfoMenu()->GetShortcut(event->key());
    if (shortcut && shortcut->key() != QKeySequence(unused_key)) {
      shortcut->setObjectName("From KeyReleaseEvent");
      emit shortcut->activated();
    }
  } else if (state == Controller::MenuType::kPause) {
    shortcut = Controller::GetPauseMenu()->GetShortcut(event->key());
    if (shortcut && shortcut->key() != QKeySequence(unused_key)) {
      shortcut->setObjectName("From KeyReleaseEvent");
      emit shortcut->activated();
    }
  } else if (state == Controller::MenuType::kAttack) {
    shortcut = Controller::GetAttackMenu()->GetShortcut(event->key());
    if (shortcut && shortcut->key() != QKeySequence(unused_key)) {
      shortcut->setObjectName("From KeyReleaseEvent");
      emit shortcut->activated();
    }
  } else if (state == Controller::MenuType::kMove) {
    shortcut = Controller::GetMoveMenu()->GetShortcut(event->key());
    if (shortcut && shortcut->key() != QKeySequence(unused_key)) {
      shortcut->setObjectName("From KeyReleaseEvent");
      emit shortcut->activated();
    }
  } else if (state == Controller::MenuType::kSettings) {
    shortcut = Controller::GetSettingsMenu()->GetShortcut(event->key());
    if (shortcut && shortcut->key() != QKeySequence(unused_key)) {
      shortcut->setObjectName("From KeyReleaseEvent");
      emit shortcut->activated();
    }
  } else if (state == Controller::MenuType::kShop) {
    shortcut = Controller::GetShopMenu()->GetShortcut(event->key());
    if (shortcut && shortcut->key() != QKeySequence(unused_key)) {
      shortcut->setObjectName("From KeyReleaseEvent");
      emit shortcut->activated();
    }
  }
}

void EventHandler::View::MoveTo() {
  double width = view_->sceneRect().width();
  double height = view_->sceneRect().height();
  QPointF direction = 2 * target_position_ - view_->sceneRect().center();
  double distance =
      sqrt(direction.x() * direction.x() + direction.y() * direction.y());

  double velocity_coefficient = 5;
  if (is_scaled_motion) {
    velocity_coefficient = 30;
  }
  const double kVelocity = width / velocity_coefficient;

  double time = distance / kVelocity;
  if (distance > kVelocity) {
    view_->setSceneRect(
        view_->sceneRect().x() + direction.x() / static_cast<int>(time + 1),
        view_->sceneRect().y() + direction.y() / static_cast<int>(time + 1),
        width, height);
  }
  if (view_->matrix().m11() < kMaxScale && is_scaled_motion) {
    double scale_velocity;
    if (abs(time) < 1e-12) {
      // TODO
      // Выбрать скорость зума
      scale_velocity = (kMaxScale - view_->matrix().m11()) *
                           (kMaxScale - view_->matrix().m11()) * 0.1 +
                       0.01;
    } else {
      scale_velocity = (kMaxScale - view_->matrix().m11()) / time;
    }
    QMatrix matrix;
    matrix.setMatrix(view_->matrix().m11() + scale_velocity,
                     view_->matrix().m12(), view_->matrix().m21(),
                     view_->matrix().m22() + scale_velocity,
                     view_->matrix().dx(), view_->matrix().dy());
    view_->setMatrix(matrix);
    Controller::GetGameMenu()->ReDraw();
  }
  if (distance <= kVelocity &&
      (view_->matrix().m11() >= kMaxScale || !is_scaled_motion)) {
    view_->setSceneRect(2 * target_position_.x() - width / 2,
                        2 * target_position_.y() - height / 2, width, height);
    delete timer_;
    timer_ = nullptr;
    target_ = nullptr;
    if (is_scaled_motion) {
      Controller::GetGameMenu()->ReDraw();
      current_motion_ = MotionType::kNoMotion;
      Controller::SwitchMenu(Controller::MenuType::kPlanet);
    } else {
      QTimer::singleShot(500, this, SLOT(ChangePlanet()));
    }
  }
}

void EventHandler::View::Scale(QWheelEvent* event) {
  if (Controller::GetMenuType() != Controller::MenuType::kGame &&
      Controller::GetMenuType() != Controller::MenuType::kPlanet) {
    return;
  }

  double current_scale = view_->matrix().m11();
  int8_t direction = 1;
  if (event->delta() < 0) {
    direction = -1;
  }

  if ((!CompareMotion(MotionType::kScale) &&
       current_motion_ != MotionType::kNoMotion) ||
      (current_scale <= kMinScale && event->delta() < 0) ||
      (current_scale >= kMaxScale && event->delta() > 0)) {
    return;
  }
  if (direction != scale_direction_) {
    current_motion_ = MotionType::kNoMotion;
    delete timer_;
    timer_ = nullptr;
    scale_direction_ = 0;
    goal_scale_ = current_scale;
  }
  if (timer_ == nullptr) {
    current_motion_ = MotionType::kScale;
    const double kScale = event->delta() * current_scale / 200;
    scale_direction_ = direction;
    if (scale_direction_ > 0) {
      goal_scale_ = std::min(kMaxScale, current_scale + kScale);
    } else {
      goal_scale_ = std::max(kMinScale, current_scale + kScale);
    }
    timer_ = new QTimer();
    timer_->start(15);
    connect(timer_, SIGNAL(timeout()), this, SLOT(ScaleToGoal()));
  }
}

void EventHandler::View::GeneratePath(
    QVector<std::pair<Planet*, Planet*>> planets_to_show) {
  for (const auto& planet_to_show : planets_to_show) {
    planets_to_show_.push_back(
        std::make_pair(planet_to_show.first->GetCoordinates() * 2,
                       planet_to_show.second->GetCoordinates() * 2));
  }
  if (!planets_to_show.empty()) {
    planets_to_show_.push_back(
        std::make_pair(view_->sceneRect().center() / 2, QPointF(0, 0)));
  }
}

void EventHandler::View::ShowBotsAttack() {
  if (planets_to_show_.size() <= motion_counter) {
    current_motion_ = MotionType::kNoMotion;
    motion_counter = 0;
    Controller::scene->UpdatePlanetsGraph();
    Controller::GetGameMenu()->Show();
    planets_to_show_.clear();
    return;
  }

  if (planets_to_show_.size() <= motion_counter + 1) {
    target_position_ = planets_to_show_[motion_counter].first;
    StartMotion();
    return;
  }

  target_ = dynamic_cast<QGraphicsItem*>(Controller::scene->itemAt(
      planets_to_show_[motion_counter].first, QTransform()));
  if (target_ == nullptr) {
    int32_t i = motion_counter;
    for (; i < planets_to_show_.size(); i++) {
      target_ = dynamic_cast<QGraphicsItem*>(Controller::scene->itemAt(
          planets_to_show_[motion_counter].first, QTransform()));
      if (target_ != nullptr) {
        motion_counter = i;
        break;
      }
    }
    if (i == planets_to_show_.size() && target_ == nullptr) {
      motion_counter = 0;
      planets_to_show_.clear();
      return;
    }
  }
  target_position_ = target_->pos();
  StartMotion();
}

void EventHandler::View::StartMotion() {
  current_motion_ = MotionType::kBotsAttack;
  timer_ = new QTimer();
  timer_->start(15);
  is_scaled_motion = false;
  connect(timer_, SIGNAL(timeout()), this, SLOT(MoveTo()));
}

void EventHandler::View::ChangePlanet() {
  PlanetGraphics* planet =
      dynamic_cast<PlanetGraphics*>(Controller::scene->itemAt(
          planets_to_show_[motion_counter].second, QTransform()));
  if (planet != nullptr) {
    Controller::scene->GetGraph()->UpdatePlanet(planet);
  }
  motion_counter++;
  QTimer::singleShot(500, this, SLOT(ShowBotsAttack()));
}

EventHandler::View::MotionType EventHandler::View::GetMotionType() {
  return current_motion_;
}

void EventHandler::View::ScaleToGoal() {
  double current_scale = view_->matrix().m11();
  double scale_velocity = kScaleVelocity * scale_direction_ * current_scale;

  if ((scale_direction_ > 0 && current_scale >= goal_scale_) ||
      (scale_direction_ < 0 && current_scale <= goal_scale_) ||
      (goal_scale_ < kMinScale) || (goal_scale_ > kMaxScale)) {
    current_motion_ = MotionType::kNoMotion;
    delete timer_;
    timer_ = nullptr;
    scale_direction_ = 0;
    goal_scale_ = current_scale;
    return;
  }

  QMatrix matrix;
  matrix.setMatrix(current_scale + scale_velocity, view_->matrix().m12(),
                   view_->matrix().m21(), current_scale + scale_velocity,
                   view_->matrix().dx(), view_->matrix().dy());
  view_->setMatrix(matrix);

  Controller::GetGameMenu()->ReDraw();
}
