#include "menu.h"

#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPushButton>
#include <QTimer>
#include <memory>

#include "core/menugraph.h"
#include "core/statemachine.h"
#include "data/loader.h"
#include "graphics/attackresultwindow.h"
#include "graphics/buttonitem.h"
#include "graphics/imageitem.h"
#include "graphics/planetgraphics.h"
#include "graphics/planetinfographics.h"
#include "graphics/unitwidget.h"
#include "mainwindow.h"
#include "objects/planet.h"
#include "objects/player.h"
#include "scene/gamescene.h"
#include "scene/gameview.h"
#include "util/typeoffset.h"

MainMenu::MainMenu() {
  connect(this, SIGNAL(btnExitClick()), Controller::window, SLOT(Exit()));
  this->Draw();
}

MainMenu::~MainMenu() {
  Controller::scene->removeItem(txt_total_space_);
  Controller::scene->removeItem(btn_exit_);
  Controller::scene->removeItem(btn_new_game_);
}

void MainMenu::SetZValue() {
  txt_total_space_->setZValue(ZValues::kMainMenu);
  btn_new_game_->setZValue(ZValues::kMainMenu);
  btn_exit_->setZValue(ZValues::kMainMenu);
}

void MainMenu::Draw() {
  GameView* view = Controller::view;

  btn_exit_ =
      new ImageItem(Loader::GetButtonImage(ButtonsEnum::kExitButton),
                    static_cast<int>(kWidth / (5 * view->matrix().m11())),
                    static_cast<int>(kHeight / (12 * view->matrix().m11())));
  btn_new_game_ =
      new ImageItem(Loader::GetButtonImage(ButtonsEnum::kNewGameButton),
                    static_cast<int>(kWidth / (5 * view->matrix().m11())),
                    static_cast<int>(kHeight / (12 * view->matrix().m11())));
  txt_total_space_ =
      new ImageItem(Loader::GetButtonImage(ButtonsEnum::kBackground),
                    static_cast<int>(kWidth / view->matrix().m11()),
                    static_cast<int>(kHeight / view->matrix().m11()));

  SetZValue();

  Controller::scene->addItem(txt_total_space_);
  Controller::scene->addItem(btn_exit_);
  Controller::scene->addItem(btn_new_game_);

  QPointF cp = Controller::view->sceneRect().center() / 2;

  btn_new_game_->setPos(cp - QPointF(0, kHeight / 49) / view->matrix().m11());
  btn_exit_->setPos(cp + QPointF(0, kHeight / 28) / view->matrix().m11());
  txt_total_space_->setPos(cp);
}

void MainMenu::SwitchTo(Controller::MenuType menu) {
  if (!Controller::Graph()->HasConnection(Controller::GetMenuType(), menu)) {
    return;
  }
  Controller::SetMainMenu(nullptr);

  if (menu == Controller::MenuType::kGame) {
    Controller::SetGameMenu(new GameMenu());
    Controller::SetMenuType(Controller::MenuType::kGame);
  }
}

PauseMenu::PauseMenu() { this->Draw(); }

PauseMenu::~PauseMenu() {
  Controller::scene->removeItem(btn_back_);
  Controller::scene->removeItem(btn_exit_);
  Controller::scene->removeItem(background_rect_);
}

void PauseMenu::SetZValue() {
  background_rect_->setZValue(ZValues::kPauseMenu);
  btn_back_->setZValue(ZValues::kPauseMenu);
  btn_exit_->setZValue(ZValues::kPauseMenu);
}

void PauseMenu::Draw() {
  GameView* view = Controller::view;
  QPointF center =
      view->mapToScene(QPoint(view->rect().width(), view->rect().height()) / 2);

  QRectF rect = view->sceneRect();

  rect.setX(center.x() - view->rect().width() / view->matrix().m11());
  rect.setY(center.y() - view->rect().height() / view->matrix().m11());
  rect.setSize((rect.size() * 4) / view->matrix().m11());

  background_rect_ = new QGraphicsRectItem();
  background_rect_->setRect(rect);
  background_rect_->setOpacity(0.7);
  background_rect_->setBrush(QColor(Qt::black));

  btn_back_ =
      new ImageItem(Loader::GetButtonImage(ButtonsEnum::kBackToGameButton),
                    static_cast<int>(kWidth / (5 * view->matrix().m11())),
                    static_cast<int>(kHeight / (12 * view->matrix().m11())));
  btn_exit_ =
      new ImageItem(Loader::GetButtonImage(ButtonsEnum::kToMenuButton),
                    static_cast<int>(kWidth / (5 * view->matrix().m11())),
                    static_cast<int>(kHeight / (12 * view->matrix().m11())));

  SetZValue();

  Controller::scene->addItem(background_rect_);
  Controller::scene->addItem(btn_back_);
  Controller::scene->addItem(btn_exit_);

  btn_back_->setPos(view->sceneRect().center() / 2 -
                    QPoint(0, static_cast<int>(kHeight / 14)) /
                        view->matrix().m11());
  btn_exit_->setPos(btn_back_->pos() +
                    QPoint(0, static_cast<int>(kHeight / 18)) /
                        view->matrix().m11());
}

void PauseMenu::SwitchTo(Controller::MenuType menu) {
  if (!Controller::Graph()->HasConnection(Controller::GetMenuType(), menu)) {
    return;
  }

  if (menu == Controller::MenuType::kGame) {
    Controller::SetPauseMenu(nullptr);
    Controller::SetMenuType(Controller::MenuType::kGame);
  }

  if (menu == Controller::MenuType::kMain) {
    Controller::SetPauseMenu(nullptr);
    Controller::SetGameMenu(nullptr);
    Controller::SetMainMenu(new MainMenu());
    Controller::SetMenuType(Controller::MenuType::kMain);
  }
}

PlanetMenu::PlanetMenu() {
  if (Controller::GetActivePlanet()->GetOwner() ==
      Controller::scene->GetPlayer()) {
    // Выставление картинок соответственно их действию
    btn1_ = new ImageItem(Loader::GetButtonImage(ButtonsEnum::kSimpleButton),
                          kWidth / 12, kHeight / 15);
    button_to_menu_[btn1_] = Controller::MenuType::kMove;
    btn2_ = new ImageItem(Loader::GetButtonImage(ButtonsEnum::kSimpleButton),
                          kWidth / 12, kHeight / 15);
    button_to_menu_[btn2_] = Controller::MenuType::kGame;
    btn3_ = new ImageItem(Loader::GetButtonImage(ButtonsEnum::kSimpleButton),
                          kWidth / 12, kHeight / 15);
    button_to_menu_[btn3_] = Controller::MenuType::kGame;
  } else {
    btn1_ = new ImageItem(Loader::GetButtonImage(ButtonsEnum::kSimpleButton),
                          kWidth / 12, kHeight / 15);
    button_to_menu_[btn1_] = Controller::MenuType::kAttack;
    btn2_ = new ImageItem(Loader::GetButtonImage(ButtonsEnum::kSimpleButton),
                          kWidth / 12, kHeight / 15);
    button_to_menu_[btn2_] = Controller::MenuType::kGame;
    btn3_ = new ImageItem(Loader::GetButtonImage(ButtonsEnum::kSimpleButton),
                          kWidth / 12, kHeight / 15);
    button_to_menu_[btn3_] = Controller::MenuType::kGame;
  }
  this->Draw();
  Controller::scene->UpdatePlanetsGraph();
}

PlanetMenu::~PlanetMenu() {
  Controller::scene->removeItem(btn1_);
  Controller::scene->removeItem(btn2_);
  Controller::scene->removeItem(btn3_);
}

void PlanetMenu::SetZValue() {
  btn1_->setZValue(ZValues::kPlanetMenu);
  btn2_->setZValue(ZValues::kPlanetMenu);
  btn3_->setZValue(ZValues::kPlanetMenu);
}

void PlanetMenu::Draw() {
  SetZValue();
  Controller::scene->addItem(btn1_);
  Controller::scene->addItem(btn2_);
  Controller::scene->addItem(btn3_);

  // TODO определить зависимость множителя от радиуса планеты
  Planet* p = Controller::GetActivePlanet();
  radius_ = -(p->GetRadius() * 1.3 / 2);
  QPointF vec1(0, radius_);
  QPointF vec2(vec1 * QTransform().rotate(60));
  QPointF vec3(vec1 * QTransform().rotate(-60));

  btn1_->setPos(p->GetCoordinates() + vec1);
  btn2_->setPos(p->GetCoordinates() + vec2);
  btn3_->setPos(p->GetCoordinates() + vec3);

  btn1_->setY(btn1_->y() - radius_ / 48);
  btn2_->setX(btn2_->x() + radius_ / 48);
  btn3_->setX(btn3_->x() - radius_ / 48);
}

void PlanetMenu::Hide() {
  btn1_->hide();
  btn2_->hide();
  btn3_->hide();
}

void PlanetMenu::Show() {
  btn1_->show();
  btn2_->show();
  btn3_->show();
}

Controller::MenuType PlanetMenu::GetNextMenu(ImageItem* button) const {
  if (button_to_menu_.find(button) != button_to_menu_.end()) {
    return button_to_menu_.at(button);
  }
  return Controller::MenuType::kPlanet;
}

void PlanetMenu::SwitchTo(Controller::MenuType menu) {
  if (!Controller::Graph()->HasConnection(Controller::GetMenuType(), menu)) {
    return;
  }
  switch (menu) {
    case Controller::MenuType::kGame: {
      Controller::SetPlanetMenu(nullptr);
      Controller::SetMenuType(Controller::MenuType::kGame);
      break;
    }
    case Controller::MenuType::kAttack: {
      Controller::SetPlanetMenu(nullptr);
      Controller::SetAttackMenu(new AttackMenu());
      Controller::SetMenuType(Controller::MenuType::kAttack);
      break;
    }
    case Controller::MenuType::kMove: {
      Controller::SetPlanetMenu(nullptr);
      Controller::SetMoveMenu(new MoveMenu());
      Controller::SetMenuType(Controller::MenuType::kMove);
      break;
    }
    default: {
      break;
    }
  }
}

UnitMenu::UnitMenu() { this->Draw(); }

UnitMenu::~UnitMenu() {}

void UnitMenu::Draw() {}

void UnitMenu::SwitchTo(Controller::MenuType) {}

GameMenu::GameMenu() { this->Draw(); }

GameMenu::~GameMenu() { Controller::scene->Destroy(); }

void GameMenu::SetZValue() {}

void GameMenu::SwitchTo(Controller::MenuType menu) {
  if (!Controller::Graph()->HasConnection(Controller::GetMenuType(), menu)) {
    return;
  }

  if (menu == Controller::MenuType::kPlanet) {
    Controller::SetPlanetMenu(new PlanetMenu());
    Controller::SetMenuType(Controller::MenuType::kPlanet);
  }

  if (menu == Controller::MenuType::kPause) {
    if (Controller::view->EventHandler()->GetMotionType() !=
        EventHandler::View::MotionType::kNoMotion) {
      return;
    }
    Controller::SetPauseMenu(new PauseMenu());
    Controller::SetMenuType(Controller::MenuType::kPause);
  }
}

void GameMenu::Draw() {
  Controller::view->SetNewGameSettings();
  Controller::scene->NewGame();
}

UnitsInteractionMenu::UnitsInteractionMenu() {
  PlanetGraphics* planet_graphics =
      dynamic_cast<PlanetGraphics*>(Controller::scene->itemAt(
          Controller::GetActivePlanet()->GetCoordinates() * 2, QTransform()));
  if (planet_graphics != nullptr) {
    int32_t planet_info_width = static_cast<int32_t>(
        kWidth * kSizeCoefficient / Controller::view->matrix().m11() -
        kWidth / 3 / Controller::view->matrix().m11() +
        kWidth * (1 - kSizeCoefficient) / 2);
    int32_t planet_info_height = static_cast<int32_t>(
        kHeight * (1 - 2 * kScrollPosition) - kHeight / 10);

    planet_info_ = new PlanetInfoGraphics(
        planet_graphics->GetImage(), planet_info_width, planet_info_height);
  }

  background_rect_ = new QGraphicsRectItem();
  interaction_button_ = new ButtonItem(button_width_, button_height_);
  cancel_button_ = new ButtonItem(button_width_, button_height_);
  std::map<Planet*, QVector<UnitType>> nearest_units =
      Controller::scene->GetNearestUnits(Controller::scene->GetPlayer());
  for (const auto& planet_to_units : nearest_units) {
    for (const auto& unit : planet_to_units.second) {
      unit_widgets_.push_back(std::make_shared<UnitWidget>(
          this, planet_to_units.first, unit, kUnitCellWidth, kUnitCellHeight));
    }
  }
  connect(cancel_button_, SIGNAL(clicked()), this, SLOT(Close()));
  connect(interaction_button_, SIGNAL(clicked()), this, SLOT(Interact()));
  Draw();
}

UnitsInteractionMenu::~UnitsInteractionMenu() { Destroy(); }

void UnitsInteractionMenu::SetZValue() {
  background_rect_->setZValue(ZValues::kInteractionMenu);
  interaction_button_->setZValue(ZValues::kInteractionMenu);
  cancel_button_->setZValue(ZValues::kInteractionMenu);
  planet_info_->setZValue(ZValues::kInteractionMenu);
}

void UnitsInteractionMenu::Draw() {
  QPointF coordinates = Controller::GetActivePlanet()->GetCoordinates();
  QSize size(Controller::scene->GetWidth(), Controller::scene->GetHeight());
  size *= kSizeCoefficient / Controller::view->matrix().m11();

  QRectF background_rect(
      2 * (coordinates - QPointF(size.width(), size.height()) / 4), size);
  background_rect_->setRect(background_rect);
  background_rect_->setPen(QColor(Qt::black));
  background_rect_->setBrush(QColor(Qt::black));

  scroll_scene_ = new QGraphicsScene();
  scroll_scene_->setBackgroundBrush(QColor(Qt::black));
  scroll_view_ = new ScrollingView(scroll_scene_, Controller::window);
  // Выставление положение и размер относительно экрана,
  // т.е. левый верхний угол - (0, 0)
  scroll_view_->setGeometry(
      static_cast<int32_t>(kScrollPosition * kWidth),
      static_cast<int32_t>(kScrollPosition * kHeight), kUnitCellWidth + 5,
      std::min(kUnitCellHeight * unit_widgets_.size(),
               static_cast<int32_t>(kHeight * (1 - 2 * kScrollPosition) + 1)));
  // Область внутри, которая скроллится, выставляется в зависимости от размеров
  // и количества виджетов
  scroll_view_->setSceneRect(0, 0, kUnitCellWidth + 5,
                             kUnitCellHeight * unit_widgets_.size() + 1);
  int32_t y = 0;
  for (const auto& unit : unit_widgets_) {
    unit.get()->setPos(0, y);
    scroll_scene_->addItem(unit.get());
    y += kUnitCellHeight / 2;
  }

  const int32_t distance_between = static_cast<int32_t>(
      (background_rect.width() * Controller::view->matrix().m11() -
       ((kScrollPosition - (1 - kSizeCoefficient) / 2) * kWidth +
        kUnitCellWidth + 2 * button_width_)) /
      3);
  const int32_t attack_x =
      static_cast<int32_t>(kWidth * kScrollPosition + distance_between +
                           kUnitCellWidth + button_width_ / 2);
  const int32_t attack_y = static_cast<int32_t>(
      kScrollPosition * kHeight + kHeight * (1 - 2 * kScrollPosition) -
      button_height_ + button_height_ / 2);
  interaction_button_->setPos(Controller::view->mapToScene(attack_x, attack_y));
  cancel_button_->setPos(Controller::view->mapToScene(
      static_cast<int32_t>(attack_x + button_width_ + distance_between),
      attack_y));

  planet_info_->setPos(Controller::view->mapToScene(
      static_cast<int32_t>(kWidth / 2 - kWidth / 6),
      static_cast<int32_t>(kScrollPosition * kHeight)));

  // !!!ATTENTION!!!
  // Все добавления на сцену делать в Show(), иначе фризит
  QTimer::singleShot(1, this, SLOT(Show()));
}

void UnitsInteractionMenu::SwitchTo(Controller::MenuType menu) {
  if (!Controller::Graph()->HasConnection(Controller::GetMenuType(), menu)) {
    return;
  }
  if (current_state_ == State::kResult) {
    CloseResult();
  }
  Switch(menu);
}

void UnitsInteractionMenu::ChooseUnit(UnitWidget* unit) {
  for (const auto& another_unit : unit_widgets_) {
    double another_y = another_unit.get()->y();
    if (another_y < unit->y() && another_y >= last_chosen_y_) {
      another_unit.get()->setY(another_y + kUnitCellHeight / 2);
      another_unit.get()->update();
    }
  }
  unit->setY(last_chosen_y_);
  last_chosen_y_ += kUnitCellHeight / 2;
  chosen_units_.push_back(unit);
}

void UnitsInteractionMenu::RemoveUnit(UnitWidget* unit) {
  for (const auto& another_unit : unit_widgets_) {
    double another_y = another_unit.get()->y();
    if (another_y > unit->y() && another_y < last_chosen_y_) {
      another_unit.get()->setY(another_y - kUnitCellHeight / 2);
      another_unit.get()->update();
    }
  }
  last_chosen_y_ -= kUnitCellHeight / 2;
  unit->setY(last_chosen_y_);
  chosen_units_.removeOne(unit);
}

void UnitsInteractionMenu::ShowAttackResult(
    const std::map<UnitType, int32_t>& units_to_quantity, const QString& result,
    const QString& caption) {
  attack_result_ = new AttackResultWindow(units_to_quantity, result, caption,
                                          result_width_, result_height);
  attack_result_->setPos(Controller::view->mapToScene(kWidth / 4, kHeight / 4));
  attack_result_->setZValue(ZValues::kInteractionMenu);

  result_button_ = new ButtonItem(button_width_, button_height_);
  result_button_->setPos(Controller::view->mapToScene(
      static_cast<int32_t>(kWidth / 2),
      static_cast<int32_t>(kHeight / 4 +
                           result_height * Controller::view->matrix().m11() -
                           kHeight / 15)));
  result_button_->setZValue(ZValues::kInteractionMenu);
  connect(result_button_, SIGNAL(clicked()), this, SLOT(Close()));

  Hide();

  Controller::scene->addItem(attack_result_);
  Controller::scene->addItem(result_button_);
}

void UnitsInteractionMenu::Hide() {
  scroll_view_->hide();
  background_rect_->hide();
  interaction_button_->hide();
  cancel_button_->hide();
  planet_info_->hide();
}

void UnitsInteractionMenu::Show() {
  SetZValue();
  scroll_view_->show();
  Controller::scene->addItem(background_rect_);
  Controller::scene->addItem(planet_info_);
  Controller::scene->addItem(interaction_button_);
  Controller::scene->addItem(cancel_button_);
}

void UnitsInteractionMenu::Destroy() {
  scroll_scene_->deleteLater();
  scroll_view_->deleteLater();
  Controller::scene->removeItem(cancel_button_);
  Controller::scene->removeItem(interaction_button_);
  Controller::scene->removeItem(planet_info_);
  Controller::scene->removeItem(background_rect_);
}

void UnitsInteractionMenu::Close() { SwitchTo(Controller::MenuType::kPlanet); }

void UnitsInteractionMenu::CloseResult() {
  Controller::scene->removeItem(attack_result_);
  Controller::scene->removeItem(result_button_);
}

AttackMenu::AttackMenu() : UnitsInteractionMenu() {
  // TODO
  // interaction_button_->SetPixmap()
}

void AttackMenu::Interact() {
  current_state_ = State::kResult;

  std::map<Planet*, QVector<UnitType>> planets_to_units;
  for (const auto& unit_widget : chosen_units_) {
    planets_to_units[unit_widget->GetPlanet()].push_back(
        unit_widget->GetUnit());
  }

  std::map<UnitType, int32_t> units_to_quantity;
  for (const auto& planet_to_unit : planets_to_units) {
    for (UnitType unit : planet_to_unit.second) {
      if (units_to_quantity.find(unit) == units_to_quantity.end()) {
        units_to_quantity[unit] = 1;
        continue;
      }
      units_to_quantity[unit]++;
    }
  }

  std::map<Planet*, QVector<UnitType>> all_nearest_units;
  for (const auto& planet_to_unit : planets_to_units) {
    all_nearest_units[planet_to_unit.first] = planet_to_unit.first->GetUnits();
  }

  bool is_win = Controller::GetActivePlanet()->TakeAttack(planets_to_units);
  if (is_win) {
    for (UnitType unit : Controller::GetActivePlanet()->GetUnits()) {
      units_to_quantity[unit]--;
      if (units_to_quantity[unit] == 0) {
        units_to_quantity.erase(unit);
      }
    }
    ShowAttackResult(units_to_quantity, "Winner", "They fought to the last");
  } else {
    for (const auto& planet_to_unit : planets_to_units) {
      for (UnitType unit : planet_to_unit.first->GetUnits()) {
        all_nearest_units[planet_to_unit.first].removeOne(unit);
      }
    }
    std::map<UnitType, int32_t> units_to_quantity;
    for (const auto& planet_to_unit : all_nearest_units) {
      for (UnitType unit : planet_to_unit.second) {
        if (units_to_quantity.find(unit) == units_to_quantity.end()) {
          units_to_quantity[unit] = 1;
          continue;
        }
        units_to_quantity[unit]++;
      }
    }
    ShowAttackResult(units_to_quantity, "Loser", "We won't forget them");
  }
}

void AttackMenu::Switch(Controller::MenuType menu) {
  if (menu == Controller::MenuType::kPlanet) {
    Controller::SetAttackMenu(nullptr);
    Controller::SetPlanetMenu(new PlanetMenu());
    Controller::SetMenuType(Controller::MenuType::kPlanet);
  }
}

MoveMenu::MoveMenu() : UnitsInteractionMenu() {
  // TODO
  // interaction_button_->SetPixmap();
}

void MoveMenu::Interact() {
  std::map<Planet*, QVector<UnitType>> planets_to_units;
  for (UnitWidget* unit : chosen_units_) {
    planets_to_units[unit->GetPlanet()].push_back(unit->GetUnit());
  }
  for (const auto& planet_to_unit : planets_to_units) {
    planet_to_unit.first->RemoveUnits(planet_to_unit.second);
    Controller::GetActivePlanet()->AddUnits(planet_to_unit.second);
  }
  Close();
}

void MoveMenu::Switch(Controller::MenuType menu) {
  if (menu == Controller::MenuType::kPlanet) {
    Controller::SetMoveMenu(nullptr);
    Controller::SetPlanetMenu(new PlanetMenu());
    Controller::SetMenuType(Controller::MenuType::kPlanet);
  }
}
