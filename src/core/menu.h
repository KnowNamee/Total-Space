#ifndef MENU_H
#define MENU_H

#include <QApplication>
#include <QGraphicsItem>
#include <QScreen>
#include <QWidget>

#include "core/eventhandling.h"
#include "core/statemachine.h"
#include "scene/gameview.h"
#include "util/utility.h"

class ImageItem;
class GameScene;
class MainWindow;
class UnitWidget;
class ButtonItem;
class PlanetInfoGraphics;

class Menu : public QObject {
  Q_OBJECT

 protected:
  const int32_t kWidth = qApp->screens()[0]->size().width();
  const int32_t kHeight = qApp->screens()[0]->size().height();

 public:
  virtual void SetZValue() = 0;
  virtual void Draw() = 0;
  virtual void SwitchTo(Controller::MenuType menu) = 0;
};

class MainMenu : public Menu {
  Q_OBJECT

 public:
  MainMenu();
  ~MainMenu() override;

  void SetZValue() override;
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

  void SetZValue() override;
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

  void SetZValue() override;
  void Draw() override;

  Controller::MenuType GetNextMenu(ImageItem* btn) const;

  void SwitchTo(Controller::MenuType menu) override;

 private:
  friend class EventHandler::View;

  ImageItem* btn1_;
  ImageItem* btn2_;
  ImageItem* btn3_;
  std::map<ImageItem*, Controller::MenuType> button_to_menu_;
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

  void SwitchTo(Controller::MenuType menu) override;

 private:
  friend class EventHandler::View;
};

class AttackMenu : public Menu {
  Q_OBJECT

 public:
  AttackMenu();
  ~AttackMenu() override;

  void SetZValue() override;
  void Draw() override;
  void SwitchTo(Controller::MenuType menu) override;

 private:
  friend class UnitWidget;
  void ChooseUnit(UnitWidget* unit);
  void RemoveUnit(UnitWidget* unit);

  QVector<std::shared_ptr<UnitWidget>> unit_widgets_;
  QVector<UnitWidget*> chosen_units_;
  QGraphicsRectItem* background_rect_ = nullptr;
  ButtonItem* attack_button_ = nullptr;
  ButtonItem* cancel_button_ = nullptr;
  QGraphicsScene* scroll_scene_ = nullptr;
  ScrollingView* scroll_view_ = nullptr;
  PlanetInfoGraphics* planet_info_ = nullptr;
  int32_t last_chosen_y_ = 0;

  const double kSizeCoefficient = 0.9;
  const double kScrollPosition = 0.07;
  const int32_t kUnitCellWidth = kWidth / 4;
  const int32_t kUnitCellHeight = kHeight / 5;
  const int32_t button_width_ =
      static_cast<int32_t>(kWidth / 4 / Controller::view->matrix().m11());
  const int32_t button_height_ =
      static_cast<int32_t>(kHeight / 10 / Controller::view->matrix().m11());

 private slots:
  void Show();
  void Attack();
  void Destroy();
  void Close();
};

class GameMenu : public Menu {
  Q_OBJECT

 public:
  GameMenu();
  ~GameMenu() override;

  void SetZValue() override;
  void SwitchTo(Controller::MenuType menu) override;
  void Draw() override;
  void ReDraw();
  void StartGame();

  void Hide();
  void Show();

 private:
  friend class EventHandler::View;

  ButtonItem* btn_next_;
};

#endif  // MENU_H
