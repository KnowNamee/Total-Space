#ifndef EVENTHANDLING_H
#define EVENTHANDLING_H

#include <QMouseEvent>
#include <QObject>
#include <memory>

class GameView;
class QGraphicsItem;

namespace EventHandler {

class View : public QObject {
  Q_OBJECT
 public:
  View(GameView *view);
  void MouseMoveEvent(QMouseEvent *event);
  void DoubleClick(QMouseEvent *event);
  void Scale(QWheelEvent *event);

 private:
  QGraphicsItem *target_;
  GameView *view_;
  QTimer *timer_;
  const double max_scale_ = 1;
  const double min_scale_ = 1. / 3;
  double goal_scale_;
  int8_t scale_direction_ = 0;
 private slots:
  void Move();
  void MoveTo();
  void ScaleToGoal();
};

}  // namespace EventHandler

#endif  // EVENTHANDLING_H
