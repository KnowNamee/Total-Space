#include "mainwindow.h"

#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QScreen>

#include "ui_mainwindow.h"

#include "gameview.h"
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

  StateMachine::scene = new GameScene();
  StateMachine::window = this;
  GameView *view = new GameView(StateMachine::scene, this);

  StateMachine::DrawMainMenu();

  view->setGeometry(QRect(0, 0, width, height));
  view->setSceneRect(-width / 2, -height / 2, width, height);
  view->show();
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

void MainWindow::Exit()
{
    QApplication::exit();
}

void MainWindow::StartGame()
{
    StateMachine::StartGame();
}

void MainWindow::DrawMainMenu()
{
    StateMachine::DrawMainMenu();
}

void MainWindow::RemovePauseMenu()
{
    StateMachine::RemovePauseMenu();
}
