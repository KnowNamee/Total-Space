#ifndef EVENTHANDLING_H
#define EVENTHANDLING_H

#include <QMouseEvent>
#include <QObject>
#include <memory>

class GameView;

namespace EventHandler {

class View : public QObject {
  Q_OBJECT
 public:
  View(GameView *view);
  void MouseMoveEvent(QMouseEvent *event);
//  void DoubleClick(QMouseEvent *event);

 private:
  GameView *view_;
  QTimer *timer_;

 private slots:
  void Move();
};

}  // namespace EventHandler
#endif  // EVENTHANDLING_H
