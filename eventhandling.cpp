#include "eventhandling.h"

#include <gameview.h>

#include <QDebug>
#include <QObject>
#include <QScrollBar>
#include <QTimer>
#include <cmath>

EventHandler::View::View(GameView *view) : view_(view) { timer_ = nullptr; }

void EventHandler::View::MouseMoveEvent(QMouseEvent *) {
  qreal width = view_->rect().width();
  qreal height = view_->rect().height();

  QPointF cursor = QCursor::pos();
  // TODO
  // Тоже нужно выбрать область, в которой будет двигаться экран
  if (cursor.x() > width - 100 || cursor.y() > height - 100 ||
      cursor.x() < 100 || cursor.y() < 100) {
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

void EventHandler::View::Move() {
  int32_t width = view_->rect().width();
  int32_t height = view_->rect().height();
  QPointF cursor = QCursor::pos();
  // TODO
  // Тоже нужно выбрать область, в которой будет двигаться экран
  if (cursor.x() > width - 100 || cursor.y() > height - 100 ||
      cursor.x() < 100 || cursor.y() < 100) {
    qreal x_direction = cursor.x() - width / 2;
    qreal y_direction = cursor.y() - height / 2;
    qreal scale_coeff_x =
        1. * x_direction /
        (sqrt(x_direction * x_direction + y_direction * y_direction));

    // TODO
    // Выбрать скорость перемещения
    qreal velocity = width / 160;  // 20px на моем экране

    qreal x_velocity = velocity * scale_coeff_x;

    // TODO
    // Размеры карты тоже выбрать надо
    if ((view_->sceneRect().x() >= 2 * width && x_velocity > 0) ||
        (view_->sceneRect().x() <= -2 * width && x_velocity < 0)) {
      x_velocity = 0;
    }
    qreal y_velocity = y_direction / abs(y_direction) *
                       sqrt(velocity * velocity - x_velocity * x_velocity);
    if ((view_->sceneRect().y() >= 2 * height && y_velocity > 0) ||
        (view_->sceneRect().y() <= -2 * height && y_velocity < 0)) {
      y_velocity = 0;
    }

    view_->setSceneRect(view_->sceneRect().x() + x_velocity,
                        view_->sceneRect().y() + y_velocity, width, height);
  }
}
