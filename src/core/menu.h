#ifndef MENU_H
#define MENU_H

#include <QGraphicsItem>

#include "core/eventhandling.h"
#include "core/statemachine.h"

class ImageItem;
class GameScene;
class MainWindow;

class Menu : public QObject {
  Q_OBJECT

 public:
  Menu();

  virtual void Draw();
  virtual void SwitchTo(Controller::MenuType menu);
};

class MainMenu : public Menu {
  Q_OBJECT

 public:
  MainMenu();
  ~MainMenu() override;

  void Draw() override;
  void SwitchTo(Controller::MenuType menu) override;

 signals:
  void btnExitClick();

 private:
  friend class EventHandler::View;

  ImageItem* txt_total_space_;
  ImageItem* btn_new_game_;
  ImageItem* btn_exit_;
};

class PauseMenu : public Menu {
  Q_OBJECT

 public:
  PauseMenu();
  ~PauseMenu() override;

  void Draw() override;
  void SwitchTo(Controller::MenuType menu) override;

 private:
  friend class EventHandler::View;

  ImageItem* btn_exit_;
  ImageItem* btn_back_;
  QGraphicsRectItem* background_rect_;
};

class PlanetMenu : public Menu {
  Q_OBJECT

 public:
  PlanetMenu();
  ~PlanetMenu() override;

  void Draw() override;
  void Hide();
  void Show();

  void SwitchTo(Controller::MenuType menu) override;

 private:
  friend class EventHandler::View;

  ImageItem* btn1_;
  ImageItem* btn2_;
  ImageItem* btn3_;

  double radius_;
};

class UnitMenu : public Menu {
  Q_OBJECT

 public:
  UnitMenu();
  ~UnitMenu() override;

  void Draw() override;
  void Hide();
  void Show();

 private:
  friend class EventHandler::View;
};

class GameMenu : public Menu {
  Q_OBJECT

 public:
  GameMenu();
  ~GameMenu() override;

  void SwitchTo(Controller::MenuType menu) override;
  void Draw() override;

 private:
  friend class EventHandler::View;
};

#endif  // MENU_H
