#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <QGraphicsView>
#include <QObject>
#include <memory>

#include "eventhandling.h"

class GameScene;

class GameView : public QGraphicsView {
  Q_OBJECT
 public:
  GameView(GameScene *scene, QWidget *parent);

 private:
  void mouseMoveEvent(QMouseEvent *event) override;
  void mouseDoubleClickEvent(QMouseEvent *event) override;
  void wheelEvent(QWheelEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void keyReleaseEvent(QKeyEvent *event) override;

  std::shared_ptr<EventHandler::View> event_handler_;
};

#endif  // GAMEVIEW_H
