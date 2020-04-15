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

  enum class MotionType { kMoveWithMouse, kMoveToPlanet, kScale, kNoMotion };

 public:
  View(GameView *view);

  void MouseMoveEvent(QMouseEvent *event);
  void MouseReleaseEvent(QMouseEvent *event);
  void DoubleClick(QMouseEvent *event);
  void Scale(QWheelEvent *event);

  void KeyReleaseEvent(QKeyEvent *event);

 private:
  QGraphicsItem *target_ = nullptr;
  GameView *view_;
  QTimer *timer_;
  const double kMaxScale = 1;
  const double kMinScale = 1. / 3;
  double goal_scale_;
  MotionType current_motion_ = MotionType::kNoMotion;
  int8_t scale_direction_ = 0;

  static const int kMoveZone;

  bool IsMouseInMotionZone(QPointF cursor);
  bool CompareMotion(MotionType needed_motion);

 private slots:
  void Move();
  void MoveTo();
  void ScaleToGoal();
};

}  // namespace EventHandler

#endif  // EVENTHANDLING_H
