#include "gameview.h"

#include <eventhandling.h>

#include <QDebug>

#include "gamescene.h"

GameView::GameView(GameScene *scene, QWidget *parent)
    : QGraphicsView(scene, parent) {
  setMouseTracking(true);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setFrameStyle(QFrame::NoFrame);
  scale(0.5, 0.5);
  event_handler_ = std::make_shared<EventHandler::View>(this);
}

void GameView::mouseMoveEvent(QMouseEvent *event) {
  event_handler_->MouseMoveEvent(event);
}

void GameView::mouseDoubleClickEvent(QMouseEvent *event) {
  event_handler_->DoubleClick(event);
}

void GameView::wheelEvent(QWheelEvent *event) { event_handler_->Scale(event); }
