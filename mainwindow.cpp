#include "mainwindow.h"

#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QScreen>

#include "gamescene.h"
#include "gameview.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  delete ui->mainToolBar;
  showFullScreen();

  int32_t width = qApp->screens()[0]->size().width();
  int32_t height = qApp->screens()[0]->size().height();

  // TODO
  // Это все нужно будет перенести в класс со стартовым экраном
  GameScene *game_scene = new GameScene(this);

  GameView *view = new GameView(game_scene, this);
  view->setGeometry(QRect(0, 0, width, height));
  view->setSceneRect(-width / 2, -height / 2, width, height);

  game_scene->NewGame();
  view->show();
}

MainWindow::~MainWindow() { delete ui; }
