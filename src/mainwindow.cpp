#include "mainwindow.h"

#include <QGraphicsItem>
#include <QGraphicsView>
#include <QScreen>

#include "core/menu.h"
#include "core/statemachine.h"
#include "data/loader.h"
#include "data/objectsloader.h"
#include "scene/gamescene.h"
#include "scene/gameview.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  delete ui->mainToolBar;
  showFullScreen();
  Loader::LoadAll();

  int32_t width = qApp->screens()[0]->size().width();
  int32_t height = qApp->screens()[0]->size().height();

  Controller::scene = new GameScene();
  Controller::window = this;
  Controller::LoadMenuGraph();

  Controller::view = new GameView(Controller::scene, this);
  Controller::view->setGeometry(QRect(0, 0, width, height));
  Controller::view->setSceneRect(-width / 2, -height / 2, width, height);

  Controller::SetMainMenu(new MainMenu());

  Controller::view->show();

  ObjectsLoader::LoadDataFromFile(":/objectsdata/BuildingsInfo.json");
}

MainWindow::~MainWindow() {
  Controller::Destroy();
  delete ui;
}

void MainWindow::Exit() { QApplication::exit(); }
