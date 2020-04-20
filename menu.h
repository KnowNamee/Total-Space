#ifndef MENU_H
#define MENU_H

#include <QGraphicsItem>

#include "core/eventhandling.h"

class ImageItem;
class GameScene;
class MainWindow;

class MainMenu : public QObject {
  Q_OBJECT

 public:
  MainMenu();
  ~MainMenu();

  void Draw();

 signals:
  void btnSettingsClick();
  void btnNewGameClick();
  void btnExitClick();

 private:
  friend class EventHandler::View;

  ImageItem* txt_total_space_;
  ImageItem* btn_new_game_;
  ImageItem* btn_exit_;
};

class PauseMenu : public QObject {
  Q_OBJECT

 public:
  PauseMenu();
  ~PauseMenu();

  void Draw();

 signals:
  void btnBackClick();
  void btnExitClick();

 private:
  friend class EventHandler::View;

  ImageItem* btn_exit_;
  ImageItem* btn_back_;
  QGraphicsRectItem* background_rect_;
};

class PlanetMenu : public QObject {
  Q_OBJECT

 public:
  PlanetMenu();
  ~PlanetMenu();

  void Draw();
  void Hide();
  void Show();

 signals:
  void btn1Click();
  void btn2Click();
  void btn3Click();

 private:
  friend class EventHandler::View;

  ImageItem* btn1_;
  ImageItem* btn2_;
  ImageItem* btn3_;

  double radius_;
};

class UnitMenu : public QObject {
  Q_OBJECT

 public:
  UnitMenu();
  ~UnitMenu();

  void Draw();
  void Hide();
  void Show();

 private:
  friend class EventHandler::View;
};
#endif  // MENU_H
