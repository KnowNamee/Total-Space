#include "scene/gameview.h"

#include <QDebug>
#include <QShortcut>
#include <map>

#include "core/eventhandling.h"
#include "core/keyhandler.h"
#include "core/menu.h"
#include "core/statemachine.h"
#include "graphics/buttonitem.h"
#include "graphics/unitwidget.h"
#include "scene/gamescene.h"

GameView::GameView(GameScene* scene, QWidget* parent)
    : QGraphicsView(scene, parent) {
  QCursor cursor = QCursor(QPixmap(":/Img/cursor.png"));
  setCursor(cursor);
  setMouseTracking(true);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setFrameStyle(QFrame::NoFrame);
  scale(kScaleCoefficient, kScaleCoefficient);
  event_handler_ = std::make_shared<EventHandler::View>(this);
}

GameScene* GameView::GetScene() const {
  return dynamic_cast<GameScene*>(scene());
}

void GameView::SetNewGameSettings() {
  setSceneRect(-width() / 2, -height() / 2, width(), height());
  setMatrix(QMatrix(kScaleCoefficient, matrix().m12(), matrix().m21(),
                    kScaleCoefficient, matrix().dx(), matrix().dy()));
}

void GameView::EnableKeyReleaseListener() { is_key_listener_enabled_ = true; }

bool GameView::IsKeyListenerEnabled() { return is_key_listener_enabled_; }

std::shared_ptr<EventHandler::View> GameView::EventHandler() {
  return event_handler_;
}

void GameView::mouseMoveEvent(QMouseEvent* event) {
  Q_UNUSED(event);

  event_handler_->MouseMoveEvent();
}

void GameView::mouseDoubleClickEvent(QMouseEvent* event) {
  event_handler_->DoubleClick(event);
}

void GameView::mouseReleaseEvent(QMouseEvent* event) {
  event_handler_->MouseReleaseEvent(event);
  QGraphicsView::mouseReleaseEvent(event);
}

void GameView::keyReleaseEvent(QKeyEvent* event) {
  if (is_key_listener_enabled_) {
    is_key_listener_enabled_ = false;
    KeyField* field = Controller::GetSettingsMenu()->GetActiveKeyField();
    Controller::GetSettingsMenu()->SetActiveKeyField(nullptr);
    Controller::GetKeyHandler()->UpdateKey(field, (Qt::Key)event->key());
  } else {
    event_handler_->KeyReleaseEvent(event);
  }
  QGraphicsView::keyReleaseEvent(event);
}

void GameView::wheelEvent(QWheelEvent* event) { event_handler_->Scale(event); }

ScrollingView::ScrollingView(QGraphicsScene* scene, QWidget* parent)
    : QGraphicsView(scene, parent) {
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

ScrollingView::~ScrollingView() { Controller::view->setFocus(); }

void ScrollingView::keyReleaseEvent(QKeyEvent* event) {
  Controller::view->event_handler_->KeyReleaseEvent(event);
  QGraphicsView::keyReleaseEvent(event);
}
