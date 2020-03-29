#include "mainwindow.h"

#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QScreen>

<<<<<<< HEAD
=======
#include "gamescene.h"
#include "gameview.h"
>>>>>>> fe6d6d13bbfef52aafe6ecad39dfb22f36779779
#include "ui_mainwindow.h"

#include "gamescene.h"
#include "menu.h"
#include "statemachine.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  delete ui->mainToolBar;
  showFullScreen();

  int32_t width = qApp->screens()[0]->size().width();
  int32_t height = qApp->screens()[0]->size().height();

  scene_ = new GameScene();

  main_menu_ = StateMachine::DrawMainMenu(this, scene_);
  // TODO
  // Это все нужно будет перенести в класс со стартовым экраном
  GameScene *game_scene = new GameScene(this);
  game_scene->NewGame();

  GameView *view = new GameView(game_scene, this);
  view->setGeometry(QRect(0, 0, width, height));
  view->setSceneRect(-width / 2, -height / 2, width, height);
  view->show();
}

MainWindow::~MainWindow() { delete ui; }

GameScene *MainWindow::GetScene()
{
    return scene_;
}

void MainWindow::Exit()
{
    QApplication::exit();
}
