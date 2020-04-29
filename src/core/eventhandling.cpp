#include "core/eventhandling.h"

#include <QApplication>
#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QObject>
#include <QScreen>
#include <QScrollBar>
#include <QTimer>
#include <cmath>


#include "graphics/imageitem.h"
#include "scene/gameview.h"
#include "mainwindow.h"
#include "menu.h"
#include "graphics/planetgraphics.h"
#include "core/statemachine.h"

const int EventHandler::View::kMoveZone = 32;

EventHandler::View::View(GameView* view) : view_(view) { timer_ = nullptr; }

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
  if (StateMachine::State() == StateMachine::StateGame) {
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
  int state = StateMachine::State();
  QGraphicsItem* item =
      view_->scene()->itemAt(view_->mapToScene(event->pos()), QTransform());

  if (item == nullptr) {
    if (state == StateMachine::StatePlanetMenu) {
      StateMachine::window->RemovePlanetMenu();
    }
    return;
  }

  if (state == StateMachine::StateMainMenu) {
    MainMenu* menu = StateMachine::main_menu;

    if (item->type() == ImageItem::Type) {
      ImageItem* b = dynamic_cast<ImageItem*>(item);

      if (b == menu->btn_exit_) {
        emit menu->btnExitClick();
      } else if (b == menu->btn_new_game_) {
        emit menu->btnNewGameClick();
      }
    }
  } else if (state == StateMachine::StatePauseMenu) {
    PauseMenu *menu = StateMachine::pause_menu;

    if (item->type() == ImageItem::Type) {
      ImageItem* b = dynamic_cast<ImageItem*>(item);

      if (b == menu->btn_exit_) {
        emit menu->btnExitClick();
      } else if (b == menu->btn_back_) {
        emit menu->btnBackClick();
      }
    }
  } else if (state == StateMachine::StatePlanetMenu) {
    PlanetMenu* menu = StateMachine::planet_menu;

    if (item->type() == ImageItem::Type) {
      ImageItem* b = dynamic_cast<ImageItem*>(item);
      
      if (b == menu->btn1_) {
        emit menu->btn1Click();
      } else if (b == menu->btn2_) {
        emit menu->btn2Click();
      } else if (b == menu->btn3_) {
        emit menu->btn3Click();
      }
    } else if (item->type() == PlanetGraphics::Type) {
      Planet* p = dynamic_cast<PlanetGraphics*>(item)->GetPlanet();
      if (p != StateMachine::GetActivePlanet()) {
        StateMachine::window->RemovePlanetMenu();
      }
    }
  }
}

void EventHandler::View::Move() {
  int state = StateMachine::State();
  if (state != StateMachine::StateGame) {
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
    const double kMapSize = 3;
    double x_direction = cursor.x() - width / 2;
    double y_direction = cursor.y() - height / 2;

    double scale_coeff_x =
        1. * x_direction /
        (sqrt(x_direction * x_direction + y_direction * y_direction));

    // TODO
    // Выбрать скорость перемещения
    double velocity =
        width / view_->matrix().m11() / 80;  // 20px на моем экране

    double x_velocity = velocity * scale_coeff_x;
    double y_velocity = sqrt(velocity * velocity - x_velocity * x_velocity);
    if (y_direction < 0) {
      y_velocity *= -1;
    }

    // TODO
    // Размеры карты тоже выбрать надо
    if ((view_->sceneRect().x() >= kMapSize * width && x_velocity > 0) ||
        (view_->sceneRect().x() <= -kMapSize * width && x_velocity < 0)) {
      x_velocity = 0;
    }

    if ((view_->sceneRect().y() >= kMapSize * height && y_velocity > 0) ||
        (view_->sceneRect().y() <= -kMapSize * height && y_velocity < 0)) {
      y_velocity = 0;
    }

    view_->setSceneRect(view_->sceneRect().x() + x_velocity,
                        view_->sceneRect().y() + y_velocity, width, height);
  }
}

void EventHandler::View::DoubleClick(QMouseEvent* event) {
  if (StateMachine::State() == StateMachine::StateGame) {
    QGraphicsItem* item =
        view_->scene()->itemAt(view_->mapToScene(event->pos()), QTransform());
    if (item != nullptr && timer_ == nullptr &&
        item->type() == PlanetGraphics::Type) {
      StateMachine::SetActivePlanet(
          dynamic_cast<PlanetGraphics*>(item)->GetPlanet());

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
      timer_ = new QTimer();
      timer_->start(15);
      connect(timer_, SIGNAL(timeout()), this, SLOT(MoveTo()));
    }
  }
}

void EventHandler::View::KeyReleaseEvent(QKeyEvent* event) {
  int state = StateMachine::State();
  if (state == StateMachine::StatePlanetMenu) {
    if (event->key() == Qt::Key_Escape) {
      StateMachine::window->RemovePlanetMenu();
    }
  } else if (state == StateMachine::StateGame) {
    if (event->key() == Qt::Key_Escape) {
      StateMachine::DrawPauseMenu();
    }
  } else if (state == StateMachine::StatePauseMenu) {
    if (event->key() == Qt::Key_Escape) {
      StateMachine::RemovePauseMenu();
    }
  } else if (state == StateMachine::StateUnitMenu) {
    if (event->key() == Qt::Key_Escape) {
      StateMachine::RemoveUnitMenu();
    }
  }
}

void EventHandler::View::MoveTo() {
  double width = view_->sceneRect().width();
  double height = view_->sceneRect().height();

  QPointF direction = 2 * target_->pos() - view_->sceneRect().center();
  double distance =
      sqrt(direction.x() * direction.x() + direction.y() * direction.y());

  // TODOя
  // Выбрать скорость передвижения к планете
  const double kVelocity = width / 40;

  double time = distance / kVelocity;
  if (distance > kVelocity) {
    view_->setSceneRect(
        view_->sceneRect().x() + direction.x() / static_cast<int>(time + 1),
        view_->sceneRect().y() + direction.y() / static_cast<int>(time + 1),
        width, height);
  }
  if (view_->matrix().m11() < kMaxScale) {
    double scale_velocity;
    if (abs(time) < 1e-12) {
      // TODO
      // Выбрать скорость зума
      scale_velocity = (kMaxScale - view_->matrix().m11()) *
                           (kMaxScale - view_->matrix().m11()) * 0.2 +
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
  }
  if (distance <= kVelocity && view_->matrix().m11() >= kMaxScale) {
    view_->setSceneRect(2 * target_->pos().x() - width / 2,
                        2 * target_->pos().y() - height / 2, width, height);
    current_motion_ = MotionType::kNoMotion;
    StateMachine::DrawPlanetMenu();
    delete timer_;
    timer_ = nullptr;
    target_ = nullptr;
  }
}

void EventHandler::View::Scale(QWheelEvent* event) {
  if (StateMachine::State() != StateMachine::StateGame &&
      StateMachine::State() != StateMachine::StatePlanetMenu) {
    return;
  }

  double current_scale = view_->matrix().m11();
  int8_t direction = static_cast<int8_t>(event->delta() / abs(event->delta()));

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

void EventHandler::View::ScaleToGoal() {
  double current_scale = view_->matrix().m11();
  double scale_velocity = 0.06 * scale_direction_ * current_scale;

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
}
