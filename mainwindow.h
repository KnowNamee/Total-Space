#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class MainMenu;
class PauseMenu;
class UnitMenu;
class PlanetMenu;
class GameScene;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT
 public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

  GameScene* GetScene();

 public slots:
  void Exit();

 private:
  Ui::MainWindow *ui;

  GameScene* scene_;

  MainMenu* main_menu_ = nullptr;
  PauseMenu *pause_menu_ = nullptr;
  UnitMenu *unit_menu_ = nullptr;
  PlanetMenu *planet_menu_ = nullptr;
};

#endif  // MAINWINDOW_H
