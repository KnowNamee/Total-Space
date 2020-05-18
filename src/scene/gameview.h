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
  void EnableKeyReleaseListener();
  bool IsKeyListenerEnabled();

  void ShowBotsAttack(QVector<std::pair<Planet*, Planet*>> planet_to_show);

  std::shared_ptr<EventHandler::View> EventHandler();

 private:
  friend class ScrollingView;

  void mouseMoveEvent(QMouseEvent* event) override;
  void mouseDoubleClickEvent(QMouseEvent* event) override;
  void wheelEvent(QWheelEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;
  void keyReleaseEvent(QKeyEvent* event) override;

  const double kScaleCoefficient = 0.5;
  bool is_key_listener_enabled_ = false;
  std::shared_ptr<EventHandler::View> event_handler_;
};

class ScrollingView : public QGraphicsView {
  Q_OBJECT

 public:
  ScrollingView(QGraphicsScene* scene, QWidget* parent = nullptr);
  ~ScrollingView() override;

 private:
  void keyReleaseEvent(QKeyEvent* event) override;
};

#endif  // GAMEVIEW_H
