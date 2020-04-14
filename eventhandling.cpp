#include "eventhandling.h"

#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QObject>
#include <QScrollBar>
#include <QTimer>
#include <cmath>

#include "planetgraphics.h"
#include "statemachine.h"
#include "button.h"
#include "menu.h"
#include "gameview.h"
#include "mainwindow.h"

EventHandler::View::View(GameView *view) : view_(view) { timer_ = nullptr; }

void EventHandler::View::MouseMoveEvent(QMouseEvent *) {
    int state = StateMachine::State();
    if (state == StateMachine::StateGame ||
        state == StateMachine::StatePlanetMenu) {
      double width = view_->rect().width();
      double height = view_->rect().height();

      QPointF cursor = QCursor::pos();
      // TODO
      // Тоже нужно выбрать область, в которой будет двигаться экран
      if (cursor.x() > width - width / 32 || cursor.y() > height - width / 32 ||
              cursor.x() < width / 32 || cursor.y() < width / 32) {
          if (timer_ == nullptr) {
              timer_ = new QTimer();
              timer_->start(15);
              connect(timer_, SIGNAL(timeout()), this, SLOT(Move()));
          }
      } else {
          if (timer_ != nullptr) {
              delete timer_;
              timer_ = nullptr;
          }
      }
    }
}

void EventHandler::View::MouseReleaseEvent(QMouseEvent *event) {
    int state = StateMachine::State();
    QGraphicsItem *item =
        view_->scene()->itemAt(view_->mapToScene(event->pos()), QTransform());

    if (item == nullptr) {
        if (state == StateMachine::StatePlanetMenu) {
            StateMachine::window->RemovePlanetMenu();
        }
        return;
    }

    if (state == StateMachine::StateMainMenu) {
        MainMenu* menu = StateMachine::main_menu;
        if (item->type() == Button::Type) {
            Button* b = dynamic_cast<Button*>(item);
            if (b == menu->btn_exit_) {
                emit menu->btnExitClick();
            } else if (b == menu->btn_new_game_) {
                emit menu->btnNewGameClick();
            }
        }
    } else if (state == StateMachine::StatePauseMenu) {
        PauseMenu* menu = StateMachine::pause_menu;
        if (item->type() == Button::Type) {
            Button* b = dynamic_cast<Button*>(item);
            if (b == menu->btn_exit_) {
                emit menu->btnExitClick();
            } else if (b == menu->btn_back_) {
                emit menu->btnBackClick();
            }
        }
    } else if (state  == StateMachine::StatePlanetMenu) {
        PlanetMenu* menu = StateMachine::planet_menu;
        if (item->type() == Button::Type) {
            Button* b = dynamic_cast<Button*>(item);
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
    if (StateMachine::State() == StateMachine::StatePauseMenu) {
        delete(timer_);
        timer_ = nullptr;
        return;
    }

  int32_t width = view_->rect().width();
  int32_t height = view_->rect().height();
  QPointF cursor = QCursor::pos();
  // TODO
  // Тоже нужно выбрать область, в которой будет двигаться экран
  if (cursor.x() > width - width / 32 || cursor.y() > height - width / 32 ||
      cursor.x() < width / 32 || cursor.y() < width / 32) {
    double x_direction = cursor.x() - width / 2;
    double y_direction = cursor.y() - height / 2;
    double scale_coeff_x =
        1. * x_direction /
        (sqrt(x_direction * x_direction + y_direction * y_direction));

    // TODO
    // Выбрать скорость перемещения
    double velocity = width / 80;  // 20px на моем экране

    double x_velocity = velocity * scale_coeff_x;

    // TODO
    // Размеры карты тоже выбрать надо
    if ((view_->sceneRect().x() >= 2 * width && x_velocity > 0) ||
        (view_->sceneRect().x() <= -2 * width && x_velocity < 0)) {
      x_velocity = 0;
    }
    double y_velocity = y_direction / abs(y_direction) *
                       sqrt(velocity * velocity - x_velocity * x_velocity);
    if ((view_->sceneRect().y() >= 2 * height && y_velocity > 0) ||
        (view_->sceneRect().y() <= -2 * height && y_velocity < 0)) {
      y_velocity = 0;
    }

    view_->setSceneRect(view_->sceneRect().x() + x_velocity,
                        view_->sceneRect().y() + y_velocity, width, height);
  }
}

void EventHandler::View::DoubleClick(QMouseEvent *event) {
  if (StateMachine::State() == StateMachine::StateGame) {
    QGraphicsItem *item =
        view_->scene()->itemAt(view_->mapToScene(event->pos()), QTransform());
    if (item != nullptr && timer_ == nullptr &&
        item->type() == PlanetGraphics::Type) {
      StateMachine::SetActivePlanet(
                  dynamic_cast<PlanetGraphics*>(item)->GetPlanet());
      target_ = item;
      timer_ = new QTimer();
      timer_->start(15);
      connect(timer_, SIGNAL(timeout()), this, SLOT(MoveTo()));
    }
  }
}

void EventHandler::View::KeyReleaseEvent(QKeyEvent *event) {
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
    }
}

void EventHandler::View::MoveTo() {
  int32_t width = view_->rect().width();
  int32_t height = view_->rect().height();

  double x = target_->boundingRect().x();
  double y = target_->boundingRect().y();
  double radius = target_->boundingRect().width() / 2;

  double x_direction = 2 * (x + radius) - view_->sceneRect().x() - width / 2;
  double y_direction = 2 * (y + radius) - view_->sceneRect().y() - height / 2;

  double distance = sqrt(x_direction * x_direction + y_direction * y_direction);

  // TODO
  // Выбрать скорость передвижения к планете
  double velocity = width / 40;

  if (distance > velocity) {
    double time = distance / velocity;
    view_->setSceneRect(
        view_->sceneRect().x() + x_direction / static_cast<int>(time + 1),
        view_->sceneRect().y() + y_direction / static_cast<int>(time + 1),
        width, height);
  } else {
    view_->setSceneRect(2 * (x + radius) - width / 2,
                        2 * (y + radius) - height / 2, width, height);
    delete timer_;
    timer_ = nullptr;
    StateMachine::DrawPlanetMenu();
  }
}
