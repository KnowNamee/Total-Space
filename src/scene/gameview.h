#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <QGraphicsView>
#include <QObject>
#include <memory>

#include "core/eventhandling.h"

class GameScene;

class GameView : public QGraphicsView {
  Q_OBJECT
 public:
  GameView(GameScene* scene, QWidget* parent);  
  GameScene* GetScene() const;
  void SetNewGameSettings();

  std::shared_ptr<EventHandler::View> EventHandler();

 private:
  void mouseMoveEvent(QMouseEvent* event) override;
  void mouseDoubleClickEvent(QMouseEvent* event) override;
  void wheelEvent(QWheelEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;
  void keyReleaseEvent(QKeyEvent* event) override;

  const double kScaleCoefficient = 0.5;
  std::shared_ptr<EventHandler::View> event_handler_;
};

#endif  // GAMEVIEW_H
