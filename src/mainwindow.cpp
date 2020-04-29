#include "mainwindow.h"

#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QScreen>

#include "data/objectsloader.h"
#include "scene/gamescene.h"
#include "scene/gameview.h"
#include "data/loader.h"
#include "menu.h"
#include "core/statemachine.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  delete ui->mainToolBar;
  showFullScreen();
  Loader::LoadAll();

  int32_t width = qApp->screens()[0]->size().width();
  int32_t height = qApp->screens()[0]->size().height();

  StateMachine::scene = new GameScene();
  StateMachine::window = this;

  StateMachine::view = new GameView(StateMachine::scene, this);
  StateMachine::view->setGeometry(QRect(0, 0, width, height));
  StateMachine::view->setSceneRect(-width / 2, -height / 2, width, height);

  StateMachine::DrawMainMenu();

  StateMachine::view->show();

  ObjectsLoader::LoadDataFromFile(":/obectsdata/BuildingsInfo.json");
}

MainWindow::~MainWindow() {
  if (StateMachine::unit_menu) {
    StateMachine::RemoveUnitMenu();
  }
  if (StateMachine::planet_menu) {
    StateMachine::RemovePlanetMenu();
  }
  if (StateMachine::pause_menu) {
    StateMachine::RemovePauseMenu();
  }
  if (StateMachine::main_menu) {
    StateMachine::RemoveMainMenu();
  }
  delete ui;
}

void MainWindow::Exit() { QApplication::exit(); }

void MainWindow::StartGame() { StateMachine::StartGame(); }

void MainWindow::DrawMainMenu() { StateMachine::DrawMainMenu(); }
void MainWindow::DrawUnitMenu() { StateMachine::DrawUnitMenu(); }

void MainWindow::RemovePauseMenu() { StateMachine::RemovePauseMenu(); }

void MainWindow::RemovePlanetMenu() { StateMachine::RemovePlanetMenu(); }
