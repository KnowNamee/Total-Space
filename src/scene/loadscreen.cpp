#include "loadscreen.h"

#include <QDebug>
#include <QFontDatabase>
#include <QGraphicsProxyWidget>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QGraphicsWidget>
#include <QProgressBar>
#include <QThread>

#include "core/statemachine.h"
#include "data/loader.h"
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
  const double kScale = Controller::view->matrix().m11();

  QFont fabulist_general =
      QFont(QFontDatabase::applicationFontFamilies(Loader::GetFont()).first(),
            static_cast<int32_t>(35 / kScale));

  stage_->setDefaultTextColor(Qt::white);
  // Не спрашивайте почему именно такие значения
  // мой орлиный глаз мне так подсказал.
  stage_->setPos(-70 * 2 / kScale, 0);
  stage_->setFont(fabulist_general);
  scene->addItem(stage_);

  background_ = new ImageItem(
      Loader::GetButtonImage(ButtonsEnum::kLoadingBackground), rect);
  background_->setScale(1 / view->matrix().m11());
  scene->addItem(background_);

  // !!! Константы не трогать !!!
  bar_ = GetProgressBar(w + w / 3, h / 25);
  QGraphicsProxyWidget* bar = scene->addWidget(bar_);
  bar->setPos(view->mapToScene(w / 6, (28 * h) / 49));

  bar->setZValue(ZValues::kLoadScreen);
  stage_->setZValue(ZValues::kLoadScreenStage);
  background_->setZValue(ZValues::kLoadScreen);

  QCoreApplication::processEvents();
}

LoadScreen::~LoadScreen() {
  delete (bar_);
  delete (stage_);
  delete (background_);
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
  bar->setStyleSheet(
      QString("QProgressBar {"
              "background-color: transparent;"
              "border: %1px solid white;"
              "border-radius: %2px;"
              "padding: %3px;"
              "}"
              "QProgressBar::chunk {"
              "background: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 0, stop: 0 "
              "#0e8a20, stop: 1.0 #c0c149);"
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
