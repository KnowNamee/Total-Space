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
  enum class MotionType { kMoveWithMouse, kMoveToPlanet, kScale, kNoMotion };

  View(GameView* view);

  void MouseMoveEvent();
  void MouseReleaseEvent(QMouseEvent* event);
  void DoubleClick(QMouseEvent* event);
  void Scale(QWheelEvent* event);

  MotionType GetMotionType();

  void KeyReleaseEvent(QKeyEvent* event);

 private:
  QGraphicsItem* target_ = nullptr;
  GameView* view_;
  QTimer* timer_;
  const double kMaxScale = 1;
  const double kMinScale = 1. / 3;
  double goal_scale_;
  MotionType current_motion_ = MotionType::kNoMotion;
  int8_t scale_direction_ = 0;

  const double kMoveZone = 32;
  const double kMapSize;

  bool IsMouseInMotionZone(QPointF cursor);
  bool CompareMotion(MotionType needed_motion);

 private slots:
  void Move();
  void MoveTo();
  void ScaleToGoal();
};

}  // namespace EventHandler

#endif  // EVENTHANDLING_H
