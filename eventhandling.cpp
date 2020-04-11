#include "eventhandling.h"

#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QObject>
#include <QScrollBar>
#include <QTimer>
#include <cmath>

#include "gameview.h"

EventHandler::View::View(GameView *view) : view_(view) { timer_ = nullptr; }

void EventHandler::View::MouseMoveEvent(QMouseEvent *) {
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

void EventHandler::View::Move() {
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
    double velocity =
        width / 80 / view_->matrix().m11();  // 20px на моем экране

    double x_velocity = velocity * scale_coeff_x;

    // TODO
    // Размеры карты тоже выбрать надо
    if ((view_->sceneRect().x() >= 3 * width && x_velocity > 0) ||
        (view_->sceneRect().x() <= -3 * width && x_velocity < 0)) {
      x_velocity = 0;
    }
    double y_velocity = y_direction / abs(y_direction) *
                        sqrt(velocity * velocity - x_velocity * x_velocity);
    if ((view_->sceneRect().y() >= 3 * height && y_velocity > 0) ||
        (view_->sceneRect().y() <= -3 * height && y_velocity < 0)) {
      y_velocity = 0;
    }

    view_->setSceneRect(view_->sceneRect().x() + x_velocity,
                        view_->sceneRect().y() + y_velocity, width, height);
  }
}

void EventHandler::View::DoubleClick(QMouseEvent *event) {
  QGraphicsItem *item =
      view_->scene()->itemAt(view_->mapToScene(event->pos()), QTransform());
  if (item != nullptr && timer_ == nullptr) {
    double scale_x = view_->matrix().m11();
    double scale_y = view_->matrix().m22();
    double event_x = scale_x * view_->mapToScene(event->pos()).x();
    double event_y = scale_y * view_->mapToScene(event->pos()).y();

    double distance = (event_x - 2 * scale_x * item->pos().x()) *
                          (event_x - 2 * scale_x * item->pos().x()) +
                      (event_y - 2 * scale_y * item->pos().y()) *
                          (event_y - 2 * scale_y * item->pos().y());
    if (distance > scale_x * scale_x * (item->boundingRect().height() / 2) *
                       (item->boundingRect().height() / 2)) {
      return;
    }

    target_ = item;
    timer_ = new QTimer();
    timer_->start(15);
    connect(timer_, SIGNAL(timeout()), this, SLOT(MoveTo()));
  }
}

void EventHandler::View::MoveTo() {
  double width = view_->sceneRect().width();
  double height = view_->sceneRect().height();

  double x = target_->boundingRect().x();
  double y = target_->boundingRect().y();
  double radius = target_->boundingRect().width() / 2;

  double x_direction = 2 * (x + radius) - view_->sceneRect().x() - width / 2;
  double y_direction = 2 * (y + radius) - view_->sceneRect().y() - height / 2;

  double distance = sqrt(x_direction * x_direction + y_direction * y_direction);

  // TODO
  // Выбрать скорость передвижения к планете
  const double velocity = width / 40;

  double time = distance / velocity;
  if (distance > velocity) {
    view_->setSceneRect(
        view_->sceneRect().x() + x_direction / static_cast<int>(time + 1),
        view_->sceneRect().y() + y_direction / static_cast<int>(time + 1),
        width, height);
  }
  if (view_->matrix().m11() < max_scale_) {
    double scale_velocity;
    if (abs(time) < 1e-12) {
      // TODO
      // Выбрать скорость зума
      scale_velocity = (max_scale_ - view_->matrix().m11()) *
                           (max_scale_ - view_->matrix().m11()) * 0.2 +
                       0.01;
    } else {
      scale_velocity = (max_scale_ - view_->matrix().m11()) / time;
    }
    QMatrix matrix;
    matrix.setMatrix(view_->matrix().m11() + scale_velocity,
                     view_->matrix().m12(), view_->matrix().m21(),
                     view_->matrix().m22() + scale_velocity,
                     view_->matrix().dx(), view_->matrix().dy());
    view_->setMatrix(matrix);
  }
  if (distance <= velocity && view_->matrix().m11() >= max_scale_) {
    view_->setSceneRect(2 * (x + radius) - width / 2,
                        2 * (y + radius) - height / 2, width, height);
    // TODO
    // Открытие меню планеты
    delete timer_;
    timer_ = nullptr;
  }
}

void EventHandler::View::Scale(QWheelEvent *event) {
  double current_scale = view_->matrix().m11();
  const double positive_scale = 1.3;
  const double negative_scale = 0.8;
  double final_scale = current_scale;
  if (event->delta() > 0 && current_scale >= max_scale_ / positive_scale) {
    final_scale = max_scale_;
  } else if (event->delta() > 0) {
    final_scale *= positive_scale;
  }
  if (event->delta() < 0 && current_scale <= min_scale_ / negative_scale) {
    final_scale = min_scale_;
  } else if (event->delta() < 0) {
    final_scale *= negative_scale;
  }
  QMatrix matrix;
  matrix.setMatrix(final_scale, view_->matrix().m12(), view_->matrix().m21(),
                   final_scale, view_->matrix().dx(), view_->matrix().dy());
  view_->setMatrix(matrix);
}
