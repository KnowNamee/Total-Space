#ifndef MENU_H
#define MENU_H

#include <QApplication>
#include <QGraphicsItem>
#include <QLineEdit>
#include <QScreen>
#include <QShortcut>
#include <QWidget>

#include "core/eventhandling.h"
#include "core/keyhandler.h"
#include "core/statemachine.h"
#include "graphics/keyfield.h"
#include "scene/gameview.h"
#include "util/utility.h"

class ImageItem;
class GameScene;
class MainWindow;
class UnitWidget;
class ButtonItem;
class PlanetInfoGraphics;
class FullPlanetInfo;
class AttackResultWindow;
class SettingsMenu;

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

  std::map<int, std::shared_ptr<QShortcut>> shortcuts_;

 public:
  virtual void SetZValue() = 0;
  virtual void Draw() = 0;
  virtual void SwitchTo(Controller::MenuType menu) = 0;

  QShortcut* GetShortcut(int key);
};

class MainMenu : public Menu {
  Q_OBJECT

 public:
  MainMenu();
  ~MainMenu() override;

  void SetZValue() override;
  void Draw() override;
  void SwitchTo(Controller::MenuType menu) override;

  void Hide();
  void Show();

 public slots:
  void btnNewGameClicked();
  void btnSettingsClicked();

 private:
  friend class EventHandler::View;

  ImageItem* txt_total_space_;
  ButtonItem* btn_new_game_;
  ButtonItem* btn_exit_;
  ButtonItem* btn_settings_;
};

class PauseMenu : public Menu {
  Q_OBJECT

 public:
  PauseMenu();
  ~PauseMenu() override;

  void SetZValue() override;
  void Draw() override;
  void SwitchTo(Controller::MenuType menu) override;

  void Hide();
  void Show();

 public slots:
  void btnBackClicked();
  void btnExitClicked();
  void btnSettingsClicked();
  void keyEscapeReleased();

 private:
  friend class EventHandler::View;

  ButtonItem* btn_exit_;
  ButtonItem* btn_back_;
  ButtonItem* btn_settings_;
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

  void keyEscapeReleased();
  void keyInfoReleased();
  void keyAttackReleased();
  void keyMoveReleased();

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

 private slots:
  void keyEscapeReleased();
};

class MoveMenu : public UnitsInteractionMenu {
  Q_OBJECT
 public:
  MoveMenu();

 private:
  void Interact() override;
  void Switch(Controller::MenuType menu) override;

 private slots:
  void keyEscapeReleased();
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

  QGraphicsRectItem* background_ = nullptr;
  ButtonItem* upgrade_button_ = nullptr;
  ButtonItem* exit_button_ = nullptr;
  FullPlanetInfo* planet_info_ = nullptr;

 private slots:
  void Upgrade();
  void Exit();
  void keyEscapePressed();
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
  void keyEscapeReleased();
  void keyNextReleased();

 private:
  friend class EventHandler::View;

  ButtonItem* btn_next_;
};

class Section : public QObject {
  Q_OBJECT

 public:
  Section();
  Section(const QString& name, int x, int y);
  ~Section();

  void AddData(const KeyHandler::Key& data, Controller::MenuType type);
  int GetY();

 private:
  QGraphicsTextItem* name_;
  int cur_x_;
  int cur_y_;

  std::vector<QGraphicsTextItem*> actions_;
  std::vector<KeyField*> actions_keys_;

 private slots:
  void btnChangeKeyClicked();
};

class SettingsMenu : public Menu {
  Q_OBJECT

 public:
  SettingsMenu();
  ~SettingsMenu() override;

  void SwitchTo(Controller::MenuType menu) override;
  void Draw() override;
  void SetZValue() override;

  void SetActiveKeyField(KeyField* field);
  KeyField* GetActiveKeyField();

  Controller::MenuType GetPrevMenu();
  void SetPrevMenu(Controller::MenuType menu);

 private slots:
  void keyEscapeReleased();
  void btnBackClicked();

 private:
  friend class EventHandler::View;

  QGraphicsRectItem* background_rect_;
  QGraphicsTextItem* settings_;
  QGraphicsTextItem* keypad_;
  ButtonItem* btn_back_;
  std::vector<std::shared_ptr<Section>> sections_;
  Controller::MenuType prev_menu_;

  int cur_x;
  int cur_y;
  int text_field_x;
  int text_field_y;
  KeyField* active_field_ = nullptr;
  const QString kStrNoSection = "~";

  bool AddSection(const QString& name, int x, int y);
  void DrawData();
  QString GetSectionName(Controller::MenuType type);
};

#endif  // MENU_H
