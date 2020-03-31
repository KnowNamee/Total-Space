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
  void MouseReleaseEvent(QMouseEvent *event);
  void DoubleClick(QMouseEvent *event);

  void KeyReleaseEvent(QKeyEvent* event);

 private:
  QGraphicsItem *target_;
  GameView *view_;
  QTimer *timer_;

 private slots:
  void Move();
  void MoveTo();
};

}  // namespace EventHandler

#endif  // EVENTHANDLING_H
