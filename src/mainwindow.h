#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class Menu;
class MainMenu;
class PauseMenu;
class UnitMenu;
class PlanetMenu;
class GameScene;
class Controller;

class Unit;
class Planet;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

 public slots:
  void Exit();

 private:
  Ui::MainWindow* ui;
};

#endif  // MAINWINDOW_H
