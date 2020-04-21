#include "gameview.h"

#include <QDebug>

#include "core/eventhandling.h"
#include "gamescene.h"

GameView::GameView(GameScene *scene, QWidget *parent)
    : QGraphicsView(scene, parent) {
  setMouseTracking(true);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setFrameStyle(QFrame::NoFrame);
  scale(kScaleCoefficient, kScaleCoefficient);
  event_handler_ = std::make_shared<EventHandler::View>(this);
}

void GameView::SetNewGameSettings() {
  setSceneRect(-width() / 2, -height() / 2, width(), height());
  setMatrix(QMatrix(kScaleCoefficient, matrix().m12(), matrix().m21(),
                    kScaleCoefficient, matrix().dx(), matrix().dy()));
}

void GameView::mouseMoveEvent(QMouseEvent *event) {
  event_handler_->MouseMoveEvent(event);
}

void GameView::mouseDoubleClickEvent(QMouseEvent *event) {
  event_handler_->DoubleClick(event);
}

void GameView::mouseReleaseEvent(QMouseEvent *event) {
  event_handler_->MouseReleaseEvent(event);
}

void GameView::keyReleaseEvent(QKeyEvent *event) {
  event_handler_->KeyReleaseEvent(event);
}

void GameView::wheelEvent(QWheelEvent *event) { event_handler_->Scale(event); }
