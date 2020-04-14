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

  enum class MotionType {
    kMoveWithMouse,
    kMoveToPlanet,
    kScale,
    kNoMotion
  };

 public:
  View(GameView *view);
  void MouseMoveEvent(QMouseEvent *event);
  void DoubleClick(QMouseEvent *event);
  void Scale(QWheelEvent *event);

 private:
  bool CompareMotion(MotionType needed_motion);
  bool IsMouseInMotionZone(QPointF cursor);

  QGraphicsItem *target_;
  GameView *view_;
  QTimer *timer_;
  const double kMaxScale = 1;
  const double kMinScale = 1. / 3;
  double goal_scale_;
  MotionType current_motion_ = MotionType::kNoMotion;
  int8_t scale_direction_ = 0;

  static const int kMoveZone;
 private slots:
  void Move();
  void MoveTo();
  void ScaleToGoal();
};

}  // namespace EventHandler

#endif  // EVENTHANDLING_H
