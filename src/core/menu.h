#ifndef MENU_H
#define MENU_H

#include <QApplication>
#include <QGraphicsItem>
#include <QScreen>
#include <QWidget>

#include "core/eventhandling.h"
#include "core/statemachine.h"
#include "data/objectsstorage.h"
#include "scene/gameview.h"
#include "util/utility.h"

class ImageItem;
class GameScene;
class MainWindow;
class UnitWidget;
class ButtonItem;
class ShopWidget;
class ShopPlanetInfo;
class StatusBar;
class PlanetInfoGraphics;
class FullPlanetInfo;
class AttackResultWindow;

class Menu : public QObject {
  Q_OBJECT

 protected:
  const int32_t kWidth = qApp->screens()[0]->size().width();
  const int32_t kHeight = qApp->screens()[0]->size().height();
  const double kSizeCoefficient = 0.9;
  const double kScrollPosition = 0.07;
  const int32_t kButtonWidth = kWidth / 6;
  const int32_t kButtonHeight = kHeight / 10;
  const int32_t kGeneralButtonHeight = kHeight / 13;
  const int32_t kGeneralButtonWidth = kWidth / 5;

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

 public slots:
  void btnNewGameClicked();

 private:
  friend class EventHandler::View;

  ImageItem* txt_total_space_;
  ButtonItem* btn_new_game_;
  ButtonItem* btn_exit_;
};

class PauseMenu : public Menu {
  Q_OBJECT

 public:
  PauseMenu();
  ~PauseMenu() override;

  void SetZValue() override;
  void Draw() override;
  void SwitchTo(Controller::MenuType menu) override;

 public slots:
  void btnBackClicked();
  void btnExitClicked();

 private:
  friend class EventHandler::View;

  ButtonItem* btn_exit_;
  ButtonItem* btn_back_;
  QGraphicsRectItem* background_rect_;
};

class PlanetMenu : public Menu {
  Q_OBJECT

 public:
  PlanetMenu();
  ~PlanetMenu() override;

  void SetZValue() override;
  void Draw() override;

  Controller::MenuType GetNextMenu(ButtonItem* btn) const;

  void SwitchTo(Controller::MenuType menu) override;

 public slots:
  void btnInfoClicked();
  void btnDefaultClicked();
  void btnAttackClicked();
  void btnMoveClicked();
  void btnShopClicked();

 private:
  friend class EventHandler::View;
  const int32_t kPlanetMenuButtonWidth = kWidth / 12;
  const int32_t kPlanetMenuButtonHeight = kHeight / 15;

  ButtonItem* btn1_;
  ButtonItem* btn2_;
  ButtonItem* btn3_;
  // TODO удалить button_to_menu_
  std::map<ButtonItem*, Controller::MenuType> button_to_menu_;
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

class UnitsInteractionMenu : public Menu {
  Q_OBJECT

 public:
  UnitsInteractionMenu();
  ~UnitsInteractionMenu() override;

  void SetZValue() override;
  void Draw() override;
  void SwitchTo(Controller::MenuType menu) override;

 protected:
  void ShowAttackResult(const std::map<UnitType, int32_t>& units_to_quantity,
                        const QString& result, const QString& caption);

  enum class State {
    kMain,
    kResult,
  };

  State current_state_ = State::kMain;
  QVector<UnitWidget*> chosen_units_;
  ButtonItem* interaction_button_ = nullptr;

 protected slots:
  void Close();

 private:
  friend class UnitWidget;
  void ChooseUnit(UnitWidget* unit);
  void RemoveUnit(UnitWidget* unit);
  virtual void Switch(Controller::MenuType menu) = 0;
  void Hide();

  QVector<std::shared_ptr<UnitWidget>> unit_widgets_;
  QGraphicsRectItem* background_rect_ = nullptr;
  AttackResultWindow* attack_result_ = nullptr;
  ButtonItem* cancel_button_ = nullptr;
  ImageItem* background_image_ = nullptr;
  ButtonItem* result_button_ = nullptr;
  QGraphicsScene* scroll_scene_ = nullptr;
  ScrollingView* scroll_view_ = nullptr;
  PlanetInfoGraphics* planet_info_ = nullptr;
  int32_t last_chosen_y_ = 0;

  const int32_t kUnitCellWidth = 2 * kWidth / 9;
  const int32_t kUnitCellHeight = kHeight / 5;

  const int32_t kResultWidth =
      static_cast<int32_t>(kWidth / 2 / Controller::view->matrix().m11());
  const int32_t kResultHeight =
      static_cast<int32_t>(kHeight / 2 / Controller::view->matrix().m11());

 private slots:
  void Show();
  virtual void Interact() = 0;
  void Destroy();
  void CloseResult();
};

class AttackMenu : public UnitsInteractionMenu {
  Q_OBJECT
 public:
  AttackMenu();

 private:
  void Interact() override;
  void Switch(Controller::MenuType menu) override;
};

class MoveMenu : public UnitsInteractionMenu {
  Q_OBJECT
 public:
  MoveMenu();

 private:
  void Interact() override;
  void Switch(Controller::MenuType menu) override;
};

class PlanetInfoMenu : public Menu {
  Q_OBJECT
 public:
  PlanetInfoMenu();
  ~PlanetInfoMenu() override;
  void SetZValue() override;
  void Draw() override;
  void SwitchTo(Controller::MenuType menu) override;

 private:
  void Destroy();

  ImageItem* background_ = nullptr;
  ButtonItem* upgrade_button_ = nullptr;
  ButtonItem* exit_button_ = nullptr;
  FullPlanetInfo* planet_info_ = nullptr;

 private slots:
  void Upgrade();
  void Exit();
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

private slots:
  void UpdateStatusBar();

private:
  friend class EventHandler::View;

  ButtonItem* btn_next_;
  StatusBar* status_bar_;

  const int32_t kStatusWidthCoef = 4;
  const int32_t kStatusHeightCoef = 10;
};

class ShopMenu : public Menu {
  Q_OBJECT

  enum ShopState {
    kUnits,
    kBuildings,
  };

 public:
  ShopMenu();
  ~ShopMenu() override;

  void SetZValue() override;
  void Draw() override;
  void SwitchTo(Controller::MenuType menu) override;

  void SwitchState(ShopState state);
  void MakePurchase(ShopItemType type, Resources cost, QString item_name);
  void UpdateInfo();

 private slots:
  void ChangeShop();
  void Show();
  void Close();

 private:
  friend class EventHandler::View;

  QGraphicsSimpleTextItem* text_ = nullptr;
  ShopState current_state_ = kBuildings;

  QGraphicsLineItem* border_line_ = nullptr;
  ButtonItem* buildings_btn_ = nullptr;
  ButtonItem* units_btn_ = nullptr;
  ButtonItem* exit_bnt_ = nullptr;
  ImageItem* background_image_ = nullptr;

  QGraphicsScene* shop_scene_scroll_ = nullptr;
  ScrollingView* shop_scrolling_view_ = nullptr;
  QVector<ShopWidget*> shop_buildings_;
  QVector<ShopWidget*> shop_units_;

  QGraphicsScene* info_scene_scroll_ = nullptr;
  ScrollingView* info_scrolling_view_ = nullptr;
  QVector<ShopPlanetInfo*> info_buildings_;
  QVector<ShopPlanetInfo*> info_units_;

  const double kShopSizeCoefficient = 0.9;
  const double kBorderCoefficient = 0.25;

  // группа констов отвечающихся за размер и кол-во тайлов магазина
  const int32_t kWidthCount = 3;
  const int32_t kHeightCount = 2;
  const double kWidgetWidthCoef = 1. / (kWidthCount + 1);
  const double kWidgetHeightCoef = 1. / (kHeightCount + 1);
  const int32_t kWidgetWidth = static_cast<int32_t>(
      kWidth * kSizeCoefficient * (1 - kBorderCoefficient) * kWidgetWidthCoef);
  const int32_t kWidgetHeight =
      static_cast<int32_t>(kHeight * kSizeCoefficient * kWidgetHeightCoef);
  //-------------------------------------------------------------
  const int32_t kInfoCount = 5;
  const int32_t kInfoWidth = static_cast<int32_t>(
      kWidth * kSizeCoefficient * kBorderCoefficient * kSizeCoefficient);
  const int32_t kInfoHeight = static_cast<int32_t>(
      kHeight * kSizeCoefficient * kSizeCoefficient / kInfoCount);

  const int32_t kExitBtnSize = kHeight / 20;
  const int32_t kBtnWidth = kHeight / 15;
  const int32_t kBtnHeight = kHeight / 15;
};

#endif  // MENU_H
