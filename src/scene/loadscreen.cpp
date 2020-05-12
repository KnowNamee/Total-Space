#include "loadscreen.h"

#include <QDebug>
#include <QGraphicsProxyWidget>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QGraphicsWidget>
#include <QProgressBar>
#include <QThread>

#include "core/statemachine.h"
#include "scene/gamescene.h"
#include "scene/gameview.h"
#include "util/typeoffset.h"

LoadScreen::LoadScreen(int stages_cnt) : stages_cnt_(stages_cnt) {
  stage_ = new QGraphicsTextItem();
  GameScene* scene = Controller::scene;
  GameView* view = Controller::view;
  QRectF rect = view->sceneRect();
  int h = qApp->screens()[0]->size().height();
  int w = qApp->screens()[0]->size().width();

  stage_->setDefaultTextColor(Qt::black);
  stage_->setTextWidth(w);
  stage_->setFont(QFont("Arial", 16));
  stage_->setScale(1 / view->matrix().m11());
  scene->addItem(stage_);

  // TODO ImageItem must be here
  background_.setScale(1 / view->matrix().m11());
  background_.setRect(rect);
  background_.setBrush(Qt::white);
  scene->addItem(&background_);

  // !!! Константы не трогать !!!
  bar_ = GetProgressBar(w + w / 3, h / 25);
  QGraphicsProxyWidget* bar = scene->addWidget(bar_);
  bar->setPos(view->mapToScene(w / 6, (24 * h) / 50));

  bar->setZValue(ZValues::kLoadScreen);
  stage_->setZValue(ZValues::kLoadScreenStage);
  background_.setZValue(ZValues::kLoadScreen);

  QCoreApplication::processEvents();
}

LoadScreen::~LoadScreen() {
  delete (bar_);
  delete (stage_);
}

void LoadScreen::LoadNext(const QString& msg) {
  stage_->setPlainText(msg);
  QCoreApplication::processEvents();
}

void LoadScreen::StopLoad() {
  loaded_++;
  while (bar_->value() < loaded_ * 100) {
    bar_->setValue(bar_->value() + 5);
    QCoreApplication::processEvents();
  }
}

// !!! Константы не трогать !!!
QProgressBar* LoadScreen::GetProgressBar(int w, int h) {
  QProgressBar* bar = new QProgressBar();

  int margin = (5 * h) / 50;  // was 3
  int padding = (5 * h) / 50;
  int border_width = (4 * h) / 50;
  int border_radius_out = (20 * h) / 50;  // was 25
  int border_radius_in = (10 * h) / 50;   // was 15

  bar->setFixedSize(w, h);
  bar->setStyleSheet(QString("QProgressBar {"
                             "background-color: transparent;"
                             "border: %1px solid black;"
                             "border-radius: %2px;"
                             "padding: %3px;"
                             "}"
                             "QProgressBar::chunk {"
                             "background-color: black;"
                             "margin: %4px;"
                             "border-radius: %5px;"
                             "}")
                         .arg(border_width)
                         .arg(border_radius_out)
                         .arg(padding)
                         .arg(margin)
                         .arg(border_radius_in));

  bar->setTextVisible(false);
  bar->setRange(1, stages_cnt_ * 100);
  return bar;
}
