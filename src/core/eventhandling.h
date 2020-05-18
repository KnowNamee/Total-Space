#ifndef EVENTHANDLING_H
#define EVENTHANDLING_H

#include <QMouseEvent>
#include <QObject>
#include <memory>

class GameView;
class QGraphicsItem;
class Planet;

namespace EventHandler {

class View : public QObject {
  Q_OBJECT

 public slots:
  void ShowBotsAttack();

 public:
  enum class MotionType {
    kMoveWithMouse,
    kMoveToPlanet,
    kScale,
    kNoMotion,
    kBotsAttack
  };

  View(GameView* view);

  void MouseMoveEvent();
  void MouseReleaseEvent(QMouseEvent* event);
  void DoubleClick(QMouseEvent* event);
  void Scale(QWheelEvent* event);
  void GeneratePath(QVector<std::pair<Planet*, Planet*>> planets_to_show);

  MotionType GetMotionType();

  void KeyReleaseEvent(QKeyEvent* event);

 private:
  QGraphicsItem* target_ = nullptr;
  QPointF target_position_;
  GameView* view_;
  QTimer* timer_;
  const double kMaxScale = 0.95;
  const double kMinScale = 0.3;
  double goal_scale_;
  MotionType current_motion_ = MotionType::kNoMotion;
  QVector<std::pair<QPointF, QPointF>> planets_to_show_;
  int32_t motion_counter = 0;
  int8_t scale_direction_ = 0;

  const double kMoveZone = 32;
  const double kMapSize;
  const double kScaleVelocity = 0.06;

  bool is_scaled_motion = true;

  bool IsMouseInMotionZone(QPointF cursor);
  bool CompareMotion(MotionType needed_motion);
  void StartMotion();

 private slots:
  void Move();
  void MoveTo();
  void ScaleToGoal();
  void ChangePlanet();
};

}  // namespace EventHandler

#endif  // EVENTHANDLING_H
