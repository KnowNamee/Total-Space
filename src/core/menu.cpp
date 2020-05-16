#include "menu.h"

#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPushButton>
#include <QTimer>
#include <memory>

#include "core/keyhandler.h"
#include "core/menugraph.h"
#include "core/statemachine.h"
#include "data/loader.h"
#include "graphics/attackresultwindow.h"
#include "graphics/buttonitem.h"
#include "graphics/fullplanetinfo.h"
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
  this->Draw();
  connect(btn_exit_, SIGNAL(clicked()), Controller::window, SLOT(Exit()));
  connect(btn_new_game_, SIGNAL(clicked()), this, SLOT(btnNewGameClicked()));
  connect(btn_settings_, SIGNAL(clicked()), this, SLOT(btnSettingsClicked()));
}

MainMenu::~MainMenu() {
  Controller::scene->removeItem(txt_total_space_);
  Controller::scene->removeItem(btn_exit_);
  Controller::scene->removeItem(btn_new_game_);
  Controller::scene->removeItem(btn_settings_);
}

void MainMenu::SetZValue() {
  btn_exit_->setZValue(ZValues::kMainMenu);
  txt_total_space_->setZValue(ZValues::kMainMenu);
  btn_new_game_->setZValue(ZValues::kMainMenu);
  btn_settings_->setZValue(ZValues::kMainMenu);
}

void MainMenu::Draw() {
  GameView* view = Controller::view;
  QMatrix matrix = view->matrix();
  view->setMatrix(
      QMatrix(1, matrix.m12(), matrix.m21(), 1, matrix.dx(), matrix.dy()));

  btn_exit_ = new ButtonItem(kGeneralButtonWidth, kGeneralButtonHeight);
  btn_exit_->SetPixmap(Loader::GetButtonImage(ButtonsEnum::kExitButton));

  btn_new_game_ = new ButtonItem(kGeneralButtonWidth, kGeneralButtonHeight);
  btn_new_game_->SetPixmap(Loader::GetButtonImage(ButtonsEnum::kNewGameButton));

  btn_settings_ = new ButtonItem(kGeneralButtonWidth, kGeneralButtonHeight);

  txt_total_space_ = new ImageItem(
      Loader::GetButtonImage(ButtonsEnum::kBackground),
      static_cast<int>(kWidth + 10), static_cast<int>(kHeight + 10));

  SetZValue();

  Controller::scene->addItem(txt_total_space_);
  Controller::scene->addItem(btn_exit_);
  Controller::scene->addItem(btn_new_game_);
  Controller::scene->addItem(btn_settings_);

  btn_new_game_->setPos(Controller::view->mapToScene(
      QPoint(kWidth / 2, kHeight / 2 - kHeight / 30)));
  btn_exit_->setPos(Controller::view->mapToScene(
      QPoint(kWidth / 2, kHeight / 2 + kHeight / 12)));
  btn_settings_->setPos(Controller::view->mapToScene(
      QPoint(kWidth / 2, kHeight / 2 + kHeight / 6)));

  txt_total_space_->setPos(Controller::view->sceneRect().center() / 2);
}

void MainMenu::SwitchTo(Controller::MenuType menu) {
  if (!Controller::Graph()->HasConnection(Controller::GetMenuType(), menu)) {
    return;
  }

  if (menu == Controller::MenuType::kGame) {
    Controller::SetMainMenu(nullptr);
    Controller::SetGameMenu(new GameMenu());
  }

  if (menu == Controller::MenuType::kSettings) {
    Hide();
    Controller::SetSettingsMenu(new SettingsMenu());
    Controller::GetSettingsMenu()->SetPrevMenu(Controller::MenuType::kMain);
  }

  Controller::SetMenuType(menu);
}

void MainMenu::Hide() {
  txt_total_space_->hide();
  btn_exit_->hide();
  btn_new_game_->hide();
  btn_settings_->hide();
}

void MainMenu::Show() {
  txt_total_space_->show();
  btn_exit_->show();
  btn_new_game_->show();
  btn_settings_->show();
}

void MainMenu::btnNewGameClicked() {
  Controller::SwitchMenu(Controller::MenuType::kGame);
}

void MainMenu::btnSettingsClicked() {
  Controller::SwitchMenu(Controller::MenuType::kSettings);
}

PauseMenu::PauseMenu() {
  this->Draw();
  connect(btn_back_, SIGNAL(clicked()), this, SLOT(btnBackClicked()));
  connect(btn_exit_, SIGNAL(clicked()), this, SLOT(btnExitClicked()));
  connect(btn_settings_, SIGNAL(clicked()), this, SLOT(btnSettingsClicked()));

  Controller::MenuType type = Controller::MenuType::kPause;
  KeyHandler* key_handler = Controller::GetKeyHandler();
  Qt::Key key_esc = key_handler->Get(type, Qt::Key_Escape).key;
  shortcuts_[key_esc] = std::make_shared<QShortcut>(key_esc, Controller::view);
  connect(shortcuts_[key_esc].get(), SIGNAL(activated()), this,
          SLOT(keyEscapeReleased()));
}

PauseMenu::~PauseMenu() {
  Controller::scene->removeItem(btn_back_);
  Controller::scene->removeItem(btn_exit_);
  Controller::scene->removeItem(btn_settings_);
  Controller::scene->removeItem(background_rect_);
}

void PauseMenu::SetZValue() {
  background_rect_->setZValue(ZValues::kPauseMenu);
  btn_back_->setZValue(ZValues::kPauseMenu);
  btn_exit_->setZValue(ZValues::kPauseMenu);
  btn_settings_->setZValue(ZValues::kPauseMenu);
}

void PauseMenu::Draw() {
  GameView* view = Controller::view;
  QPointF center =
      view->mapToScene(QPoint(view->rect().width(), view->rect().height()) / 2);
  double coef = view->matrix().m11();
  QRectF rect = view->sceneRect();

  rect.setX(center.x() - view->rect().width() / coef);
  rect.setY(center.y() - view->rect().height() / coef);
  rect.setSize(rect.size() * 4);

  background_rect_ = new QGraphicsRectItem();
  background_rect_->setScale(1 / coef);
  background_rect_->setRect(rect);
  background_rect_->setOpacity(0.7);
  background_rect_->setBrush(Qt::black);

  btn_back_ = new ButtonItem(kGeneralButtonWidth, kGeneralButtonHeight);
  btn_back_->SetPixmap(Loader::GetButtonImage(ButtonsEnum::kBackToGameButton));
  btn_exit_ = new ButtonItem(kGeneralButtonWidth, kGeneralButtonHeight);
  btn_exit_->SetPixmap(Loader::GetButtonImage(ButtonsEnum::kToMenuButton));
  btn_settings_ = new ButtonItem(kGeneralButtonWidth, kGeneralButtonHeight);

  SetZValue();

  Controller::scene->addItem(background_rect_);
  Controller::scene->addItem(btn_back_);
  Controller::scene->addItem(btn_exit_);
  Controller::scene->addItem(btn_settings_);

  btn_back_->setPos(Controller::view->mapToScene(
      QPoint(kWidth / 2, kHeight / 2 - kHeight / 30)));
  btn_exit_->setPos(Controller::view->mapToScene(
      QPoint(kWidth / 2, kHeight / 2 + kHeight / 12)));
  btn_settings_->setPos(Controller::view->mapToScene(
      QPoint(kWidth / 2, kHeight / 2 + kHeight / 6)));
}

void PauseMenu::SwitchTo(Controller::MenuType menu) {
  if (!Controller::Graph()->HasConnection(Controller::GetMenuType(), menu)) {
    return;
  }

  if (menu == Controller::MenuType::kGame) {
    Controller::SetPauseMenu(nullptr);
    Controller::GetGameMenu()->Show();
  } else if (menu == Controller::MenuType::kMain) {
    Controller::SetPauseMenu(nullptr);
    Controller::SetGameMenu(nullptr);
    Controller::SetMainMenu(new MainMenu());
  } else if (menu == Controller::MenuType::kSettings) {
    Hide();
    Controller::SetSettingsMenu(new SettingsMenu());
    Controller::GetSettingsMenu()->SetPrevMenu(Controller::MenuType::kPause);
  }

  Controller::SetMenuType(menu);
}

void PauseMenu::Hide() {
  btn_back_->hide();
  btn_exit_->hide();
  background_rect_->hide();
  btn_settings_->hide();
}

void PauseMenu::Show() {
  btn_back_->show();
  btn_exit_->show();
  background_rect_->show();
  btn_settings_->show();
}

void PauseMenu::btnBackClicked() {
  Controller::SwitchMenu(Controller::MenuType::kGame);
}

void PauseMenu::btnExitClicked() {
  Controller::SwitchMenu(Controller::MenuType::kMain);
}

void PauseMenu::btnSettingsClicked() {
  Controller::SwitchMenu(Controller::MenuType::kSettings);
}

void PauseMenu::keyEscapeReleased() {
  QShortcut* sc = dynamic_cast<QShortcut*>(QObject::sender());
  if (sc && sc->objectName() == "From KeyReleaseEvent") {
    sc->setObjectName("");
    Controller::SwitchMenu(Controller::MenuType::kGame);
  }
}

PlanetMenu::PlanetMenu() {
  KeyHandler* key_handler = Controller::GetKeyHandler();
  Controller::MenuType type = Controller::MenuType::kPlanet;
  Qt::Key key_btn1 = key_handler->Get(type, Qt::Key_W).key;
  Qt::Key key_btn2 = key_handler->Get(type, Qt::Key_A).key;
  Qt::Key key_btn3 = key_handler->Get(type, Qt::Key_D).key;
  Qt::Key key_esc = key_handler->Get(type, Qt::Key_Escape).key;

  if (Controller::GetActivePlanet() == nullptr ||
      Controller::GetActivePlanet()->GetOwner() !=
          Controller::scene->GetPlayer()) {
    btn1_ =
        new ButtonItem(kPlanetMenuButtonWidth, kPlanetMenuButtonHeight, false);
    btn2_ =
        new ButtonItem(kPlanetMenuButtonWidth, kPlanetMenuButtonHeight, false);
    btn3_ =
        new ButtonItem(kPlanetMenuButtonWidth, kPlanetMenuButtonHeight, false);

    btn1_->SetPixmap(Loader::GetButtonImage(ButtonsEnum::kAttackButton));
    btn2_->SetPixmap(Loader::GetButtonImage(ButtonsEnum::kSimpleButton));
    btn3_->SetPixmap(Loader::GetButtonImage(ButtonsEnum::kSimpleButton));

    shortcuts_[key_btn1] =
        std::make_shared<QShortcut>(key_btn1, Controller::window);
    shortcuts_[key_btn2] =
        std::make_shared<QShortcut>(key_btn2, Controller::window);
    shortcuts_[key_btn3] =
        std::make_shared<QShortcut>(key_btn3, Controller::window);

    connect(shortcuts_[key_btn1].get(), SIGNAL(activated()), this,
            SLOT(keyAttackReleased()));
    connect(shortcuts_[key_btn2].get(), SIGNAL(activated()), this,
            SLOT(keyEscapeReleased()));
    connect(shortcuts_[key_btn3].get(), SIGNAL(activated()), this,
            SLOT(keyInfoReleased()));
    connect(btn1_, SIGNAL(clicked()), this, SLOT(btnAttackClicked()));
    connect(btn2_, SIGNAL(clicked()), this, SLOT(btnInfoClicked()));
    connect(btn3_, SIGNAL(clicked()), this, SLOT(btnDefaultClicked()));
  } else {
    btn1_ =
        new ButtonItem(kPlanetMenuButtonWidth, kPlanetMenuButtonHeight, false);
    btn2_ =
        new ButtonItem(kPlanetMenuButtonWidth, kPlanetMenuButtonHeight, false);
    btn3_ =
        new ButtonItem(kPlanetMenuButtonWidth, kPlanetMenuButtonHeight, false);

    btn1_->SetPixmap(Loader::GetButtonImage(ButtonsEnum::kMoveButton));
    btn2_->SetPixmap(Loader::GetButtonImage(ButtonsEnum::kSimpleButton));
    btn3_->SetPixmap(Loader::GetButtonImage(ButtonsEnum::kShopButton));

    shortcuts_[key_btn1] =
        std::make_shared<QShortcut>(key_btn1, Controller::window);
    shortcuts_[key_btn2] =
        std::make_shared<QShortcut>(key_btn2, Controller::window);
    shortcuts_[key_btn3] =
        std::make_shared<QShortcut>(key_btn3, Controller::window);

    connect(shortcuts_[key_btn1].get(), SIGNAL(activated()), this,
            SLOT(keyMoveReleased()));
    connect(shortcuts_[key_btn2].get(), SIGNAL(activated()), this,
            SLOT(keyInfoReleased()));
    connect(shortcuts_[key_btn3].get(), SIGNAL(activated()), this,
            SLOT(keyEscapeReleased()));
    connect(btn1_, SIGNAL(clicked()), this, SLOT(btnMoveClicked()));
    connect(btn2_, SIGNAL(clicked()), this, SLOT(btnInfoClicked()));
    connect(btn3_, SIGNAL(clicked()), this, SLOT(btnDefaultClicked()));
  }
  shortcuts_[key_esc] =
      std::make_shared<QShortcut>(key_esc, Controller::window);
  connect(shortcuts_[key_esc].get(), SIGNAL(activated()), this,
          SLOT(keyEscapeReleased()));
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

  radius_ = -p->GetRadius() * 1.3;
  QPointF vec1(0, radius_);
  QPointF vec2(vec1 * QTransform().rotate(60));
  QPointF vec3(vec1 * QTransform().rotate(-60));

  btn1_->setPos(p->GetCoordinates() * 2 + vec1);
  btn2_->setPos(p->GetCoordinates() * 2 + vec2);
  btn3_->setPos(p->GetCoordinates() * 2 + vec3);

  btn1_->setY(btn1_->y() - radius_ / 48);
  btn2_->setX(btn2_->x() + radius_ / 48);
  btn3_->setX(btn3_->x() - radius_ / 48);
}

Controller::MenuType PlanetMenu::GetNextMenu(ButtonItem* button) const {
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
      Controller::GetGameMenu()->Show();
      break;
    }
    case Controller::MenuType::kAttack: {
      Controller::SetPlanetMenu(nullptr);
      Controller::SetAttackMenu(new AttackMenu());
      break;
    }
    case Controller::MenuType::kMove: {
      Controller::SetPlanetMenu(nullptr);
      Controller::SetMoveMenu(new MoveMenu());
      break;
    }
    case Controller::MenuType::kPlanetInfo: {
      Controller::SetPlanetMenu(nullptr);
      Controller::SetPlanetInfoMenu(new PlanetInfoMenu());
      break;
    }
    default: {
      break;
    }
  }

  Controller::SetMenuType(menu);
}

void PlanetMenu::btnInfoClicked() {
  Controller::SwitchMenu(Controller::MenuType::kPlanetInfo);
}

void PlanetMenu::btnDefaultClicked() {
  Controller::SwitchMenu(Controller::MenuType::kGame);
}

void PlanetMenu::btnAttackClicked() {
  Controller::SwitchMenu(Controller::MenuType::kAttack);
}

void PlanetMenu::btnMoveClicked() {
  Controller::SwitchMenu(Controller::MenuType::kMove);
}

void PlanetMenu::keyEscapeReleased() {
  QShortcut* sc = dynamic_cast<QShortcut*>(QObject::sender());
  if (sc && sc->objectName() == "From KeyReleaseEvent") {
    sc->setObjectName("");
    Controller::SwitchMenu(Controller::MenuType::kGame);
  }
}

void PlanetMenu::keyInfoReleased() {
  QShortcut* sc = dynamic_cast<QShortcut*>(QObject::sender());
  if (sc && sc->objectName() == "From KeyReleaseEvent") {
    sc->setObjectName("");
    Controller::SwitchMenu(Controller::MenuType::kPlanetInfo);
  }
}

void PlanetMenu::keyAttackReleased() {
  QShortcut* sc = dynamic_cast<QShortcut*>(QObject::sender());
  if (sc && sc->objectName() == "From KeyReleaseEvent") {
    sc->setObjectName("");
    Controller::SwitchMenu(Controller::MenuType::kAttack);
  }
}

void PlanetMenu::keyMoveReleased() {
  QShortcut* sc = dynamic_cast<QShortcut*>(QObject::sender());
  if (sc && sc->objectName() == "From KeyReleaseEvent") {
    sc->setObjectName("");
    Controller::SwitchMenu(Controller::MenuType::kMove);
  }
}

UnitMenu::UnitMenu() { this->Draw(); }

UnitMenu::~UnitMenu() {}

void UnitMenu::Draw() {}

void UnitMenu::SwitchTo(Controller::MenuType) {}

GameMenu::GameMenu() {
  Controller::SetMenuType(Controller::MenuType::kLoad);

  this->StartGame();
  this->Draw();

  Controller::SetMenuType(Controller::MenuType::kGame);

  Controller::MenuType type = Controller::MenuType::kGame;
  KeyHandler* key_handler = Controller::GetKeyHandler();
  Qt::Key key_esc = key_handler->Get(type, Qt::Key_Escape).key;
  Qt::Key key_next_turn = key_handler->Get(type, Qt::Key_N).key;
  shortcuts_[key_esc] = std::make_shared<QShortcut>(key_esc, Controller::view);
  shortcuts_[key_next_turn] =
      std::make_shared<QShortcut>(key_next_turn, Controller::view);

  connect(shortcuts_[key_esc].get(), SIGNAL(activated()), this,
          SLOT(keyEscapeReleased()));
  connect(shortcuts_[key_next_turn].get(), SIGNAL(activated()), this,
          SLOT(keyNextReleased()));
  connect(btn_next_, SIGNAL(clicked()), Controller::scene, SLOT(Next()));
}

GameMenu::~GameMenu() { Controller::scene->Destroy(); }

void GameMenu::SetZValue() { btn_next_->setZValue(ZValues::kGameMenu); }

void GameMenu::SwitchTo(Controller::MenuType menu) {
  if (!Controller::Graph()->HasConnection(Controller::GetMenuType(), menu)) {
    return;
  }
  Hide();

  if (menu == Controller::MenuType::kPlanet) {
    Controller::SetPlanetMenu(new PlanetMenu());
    Controller::SetMenuType(Controller::MenuType::kPlanet);
  }

  if (menu == Controller::MenuType::kPause) {
    if (Controller::view->EventHandler()->GetMotionType() !=
        EventHandler::View::MotionType::kNoMotion) {
      Show();
      return;
    }
    Controller::SetPauseMenu(new PauseMenu());
    QCoreApplication::processEvents();
    Controller::SetMenuType(Controller::MenuType::kPause);
  }
}

void GameMenu::Draw() {
  int32_t width = qApp->screens()[0]->size().width();
  int32_t height = qApp->screens()[0]->size().height();

  btn_next_ = new ButtonItem(width / 10, height / 15);
  btn_next_->SetPixmap(Loader::GetButtonImage(ButtonsEnum::kNextTurnButton));

  SetZValue();
  btn_next_->setPos(Controller::view->mapToScene(
      QPoint(width, height) - QPoint(width / 8, height / 8)));
  Controller::scene->addItem(btn_next_);
}

void GameMenu::ReDraw() {
  int32_t width = qApp->screens()[0]->size().width();
  int32_t height = qApp->screens()[0]->size().height();

  btn_next_->setPos(Controller::view->mapToScene(
      QPoint(width, height) - QPoint(width / 8, height / 8)));
}

void GameMenu::StartGame() {
  Controller::view->SetNewGameSettings();
  Controller::scene->NewGame();
}

void GameMenu::Hide() { btn_next_->hide(); }

void GameMenu::Show() { btn_next_->show(); }

void GameMenu::keyEscapeReleased() {
  QShortcut* sc = dynamic_cast<QShortcut*>(QObject::sender());
  if (sc && sc->objectName() == "From KeyReleaseEvent") {
    sc->setObjectName("");
    Controller::SwitchMenu(Controller::MenuType::kPause);
  }
}

void GameMenu::keyNextReleased() {
  QShortcut* sc = dynamic_cast<QShortcut*>(QObject::sender());
  if (sc && sc->objectName() == "From KeyReleaseEvent") {
    sc->setObjectName("");
    Controller::scene->Next();
  }
}

UnitsInteractionMenu::UnitsInteractionMenu() {
  PlanetGraphics* planet_graphics =
      dynamic_cast<PlanetGraphics*>(Controller::scene->itemAt(
          Controller::GetActivePlanet()->GetCoordinates() * 2, QTransform()));
  const double kScale = Controller::view->matrix().m11();
  if (planet_graphics != nullptr) {
    int32_t planet_info_width = static_cast<int32_t>(
        kWidth * kSizeCoefficient / kScale - kWidth / 3 / kScale +
        kWidth * (1 - kSizeCoefficient) / 2);
    int32_t planet_info_height = static_cast<int32_t>(
        (kHeight * (1 - 2 * kScrollPosition) - kHeight / 10) / kScale);

    planet_info_ = new PlanetInfoGraphics(
        planet_graphics->GetImage(), planet_info_width, planet_info_height);
  }

  interaction_button_ = new ButtonItem(kButtonWidth, kButtonHeight);
  interaction_button_->SetPixmap(
      Loader::GetButtonImage(ButtonsEnum::kBeautifulAttackButton));
  cancel_button_ = new ButtonItem(kButtonWidth, kButtonHeight);
  cancel_button_->SetPixmap(Loader::GetButtonImage(ButtonsEnum::kCancelButton));

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
  background_image_->setZValue(ZValues::kInteractionMenu);
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
  background_image_ = new ImageItem(
      Loader::GetButtonImage(ButtonsEnum::kMenuBackground), background_rect);

  scroll_scene_ = new QGraphicsScene();

  QBrush* ibrush = Loader::GetBrush();
  scroll_scene_->setBackgroundBrush(*ibrush);

  scroll_view_ = new ScrollingView(scroll_scene_, Controller::window);
  // Выставление положение и размер относительно экрана,
  // т.е. левый верхний угол - (0, 0)
  scroll_view_->setGeometry(
      static_cast<int32_t>(kScrollPosition * kWidth),
      static_cast<int32_t>(kScrollPosition * kHeight), kUnitCellWidth + 5,
      std::min(kUnitCellHeight * unit_widgets_.size(),
               static_cast<int32_t>(kHeight * (1 - 2 * kScrollPosition) + 1)));
  // Область внутри, которая скроллится, выставляется в зависимости от
  // размеров и количества виджетов
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
        kUnitCellWidth + 2 * kButtonWidth)) /
      3);
  const int32_t attack_x =
      static_cast<int32_t>(kWidth * kScrollPosition + distance_between +
                           kUnitCellWidth + kButtonWidth / 2);
  const int32_t attack_y = static_cast<int32_t>(
      kScrollPosition * kHeight + kHeight * (1 - 2 * kScrollPosition) -
      kButtonHeight + kButtonHeight / 2);
  interaction_button_->setPos(Controller::view->mapToScene(attack_x, attack_y));
  interaction_button_->SetEnabled(false);
  cancel_button_->setPos(Controller::view->mapToScene(
      static_cast<int32_t>(attack_x + kButtonWidth + distance_between),
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
  if (chosen_units_.size() == 0) {
    interaction_button_->SetEnabled(true);
  }
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
  if (chosen_units_.size() == 0) {
    interaction_button_->SetEnabled(false);
  }
}

void UnitsInteractionMenu::ShowAttackResult(
    const std::map<UnitType, int32_t>& units_to_quantity, const QString& result,
    const QString& caption) {
  attack_result_ = new AttackResultWindow(units_to_quantity, result, caption,
                                          kResultWidth, kResultHeight);
  attack_result_->setPos(Controller::view->mapToScene(kWidth / 4, kHeight / 4));
  attack_result_->setZValue(ZValues::kInteractionMenu);

  result_button_ =
      new ButtonItem(kButtonWidth, kButtonHeight - kButtonHeight / 3, true);
  result_button_->SetPixmap(Loader::GetButtonImage(ButtonsEnum::kExitButton));
  result_button_->setPos(Controller::view->mapToScene(
      static_cast<int32_t>(kWidth / 2),
      static_cast<int32_t>(kHeight / 4 +
                           kResultHeight * Controller::view->matrix().m11() -
                           kHeight / 15)));

  result_button_->setZValue(ZValues::kInteractionMenu);
  connect(result_button_, SIGNAL(clicked()), this, SLOT(Close()));

  Hide();

  Controller::scene->addItem(attack_result_);
  Controller::scene->addItem(result_button_);
}

void UnitsInteractionMenu::Hide() {
  scroll_view_->hide();
  background_image_->hide();
  interaction_button_->hide();
  cancel_button_->hide();
  planet_info_->hide();
}

void UnitsInteractionMenu::Show() {
  SetZValue();
  scroll_view_->show();
  Controller::scene->addItem(background_image_);
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
  Controller::scene->removeItem(background_image_);

  delete cancel_button_;
  delete interaction_button_;
  delete planet_info_;
  delete background_image_;
}

void UnitsInteractionMenu::Close() { SwitchTo(Controller::MenuType::kPlanet); }

void UnitsInteractionMenu::CloseResult() {
  Controller::scene->removeItem(attack_result_);
  Controller::scene->removeItem(result_button_);
}

AttackMenu::AttackMenu() : UnitsInteractionMenu() {
  // TODO
  //   interaction_button_->SetPixmap()
  KeyHandler* key_handler = Controller::GetKeyHandler();
  Qt::Key key_esc =
      key_handler->Get(Controller::MenuType::kAttack, Qt::Key_Escape).key;
  shortcuts_[key_esc] = std::make_shared<QShortcut>(key_esc, Controller::view);
  connect(shortcuts_[key_esc].get(), SIGNAL(activated()), this,
          SLOT(keyEscapeReleased()));
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
    for (UnitType unit : Controller::GetActivePlanet()->GetTiredUnits()) {
      units_to_quantity[unit]--;
      if (units_to_quantity[unit] == 0) {
        units_to_quantity.erase(unit);
      }
    }
    ShowAttackResult(units_to_quantity, "Winner", "They fought to the last");
  } else {
    for (const auto& planet_to_unit : planets_to_units) {
      for (UnitType unit : planet_to_unit.first->GetTiredUnits()) {
        all_nearest_units[planet_to_unit.first].removeOne(unit);
      }
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

void AttackMenu::keyEscapeReleased() {
  QShortcut* sc = dynamic_cast<QShortcut*>(QObject::sender());
  if (sc && sc->objectName() == "From KeyReleaseEvent") {
    sc->setObjectName("");
    SwitchTo(Controller::MenuType::kPlanet);
  }
}

MoveMenu::MoveMenu() : UnitsInteractionMenu() {
  // TODO
  // interaction_button_->SetPixmap();
  KeyHandler* key_handler = Controller::GetKeyHandler();
  Qt::Key key_esc =
      key_handler->Get(Controller::MenuType::kMove, Qt::Key_Escape).key;
  shortcuts_[key_esc] = std::make_shared<QShortcut>(key_esc, Controller::view);
  connect(shortcuts_[key_esc].get(), SIGNAL(activated()), this,
          SLOT(keyEscapeReleased()));
}

void MoveMenu::Interact() {
  std::map<Planet*, QVector<UnitType>> planets_to_units;
  for (UnitWidget* unit : chosen_units_) {
    planets_to_units[unit->GetPlanet()].push_back(unit->GetUnit());
  }
  Controller::GetActivePlanet()->MoveUnits(planets_to_units);
  Close();
}

void MoveMenu::Switch(Controller::MenuType menu) {
  if (menu == Controller::MenuType::kPlanet) {
    Controller::SetMoveMenu(nullptr);
    Controller::SetPlanetMenu(new PlanetMenu());
    Controller::SetMenuType(Controller::MenuType::kPlanet);
  }
}

void MoveMenu::keyEscapeReleased() {
  QShortcut* sc = dynamic_cast<QShortcut*>(QObject::sender());
  if (sc && sc->objectName() == "From KeyReleaseEvent") {
    sc->setObjectName("");
    SwitchTo(Controller::MenuType::kPlanet);
  }
}

PlanetInfoMenu::PlanetInfoMenu() {
  background_ = new QGraphicsRectItem;
  if (Controller::GetActivePlanet()->GetOwner() ==
      Controller::scene->GetPlayer()) {
    upgrade_button_ = new ButtonItem(kButtonWidth, kButtonHeight, true);
    connect(upgrade_button_, SIGNAL(clicked()), this, SLOT(Upgrade()));
  }

  exit_button_ = new ButtonItem(kButtonWidth, kButtonHeight, true);

  KeyHandler* key_handler = Controller::GetKeyHandler();
  Controller::MenuType type = Controller::MenuType::kMove;
  Qt::Key key_esc = key_handler->Get(type, Qt::Key_Escape).key;
  shortcuts_[key_esc] =
      std::make_shared<QShortcut>(key_esc, Controller::window);

  connect(exit_button_, SIGNAL(clicked()), this, SLOT(Exit()));
  connect(shortcuts_[key_esc].get(), SIGNAL(activated()), this,
          SLOT(keyEscapePressed()));

  Draw();
}

PlanetInfoMenu::~PlanetInfoMenu() { Destroy(); }

void PlanetInfoMenu::SetZValue() {
  background_->setZValue(ZValues::kPlanetInfo);
  if (upgrade_button_ != nullptr) {
    upgrade_button_->setZValue(ZValues::kPlanetInfo);
  }
  exit_button_->setZValue(ZValues::kPlanetInfo);
  planet_info_->setZValue(ZValues::kPlanetInfo);
}

void PlanetInfoMenu::Draw() {
  const double kScale = Controller::view->matrix().m11();
  QPointF coordinates = Controller::GetActivePlanet()->GetCoordinates();
  QSize size(Controller::scene->GetWidth(), Controller::scene->GetHeight());
  size *= kSizeCoefficient / kScale;

  QRectF background_rect(
      2 * (coordinates - QPointF(size.width(), size.height()) / 4), size);
  background_->setRect(background_rect);
  background_->setPen(QColor(Qt::black));
  background_->setBrush(QColor(Qt::black));
  Controller::scene->addItem(background_);

  const double kLeftTopCornerCoeffient = (1 - kSizeCoefficient) / 2;
  const int32_t upgrade_x = static_cast<int32_t>(
      kLeftTopCornerCoeffient * kWidth + background_rect.width() * kScale / 6);
  const int32_t button_y = static_cast<int32_t>(
      2 * kLeftTopCornerCoeffient * kHeight +
      background_rect.height() * kScale - kButtonHeight - kHeight / 30);

  if (upgrade_button_ != nullptr) {
    upgrade_button_->setPos(Controller::view->mapToScene(upgrade_x, button_y));
    Controller::scene->addItem(upgrade_button_);
    Resources upgrade = Controller::GetActivePlanet()->GetUpgradeCost();
    if (!(Controller::GetActivePlanet()->GetOwner()->GetTools() >
              upgrade.GetTools() &&
          Controller::GetActivePlanet()->GetOwner()->GetBatteries() >
              upgrade.GetBatteries())) {
      upgrade_button_->SetEnabled(false);
      // TODO
      // установка другой картинки для кнопки
    }
  }

  const int32_t exit_x = static_cast<int32_t>(
      upgrade_x + background_rect.width() * kScale * 2 / 3);
  exit_button_->setPos(Controller::view->mapToScene(exit_x, button_y));
  Controller::scene->addItem(exit_button_);

  planet_info_ = new FullPlanetInfo(
      static_cast<int32_t>(background_rect.width() * kScale),
      static_cast<int32_t>(background_rect.height() * kScale - kHeight / 30 -
                           kButtonHeight),
      Controller::GetActivePlanet());
  planet_info_->setPos(Controller::view->mapToScene(
      (upgrade_x + exit_x) / 2,
      static_cast<int32_t>((button_y + kLeftTopCornerCoeffient * kHeight) / 2 -
                           kHeight / 20)));

  Controller::scene->addItem(planet_info_);
  SetZValue();
}

void PlanetInfoMenu::SwitchTo(Controller::MenuType menu) {
  if (!Controller::Graph()->HasConnection(Controller::GetMenuType(), menu)) {
    return;
  }

  if (menu == Controller::MenuType::kPlanet) {
    Controller::SetPlanetInfoMenu(nullptr);
    Controller::SetPlanetMenu(new PlanetMenu());
  }

  Controller::SetMenuType(Controller::MenuType::kPlanet);
}

void PlanetInfoMenu::Destroy() {
  if (upgrade_button_ != nullptr) {
    Controller::scene->removeItem(upgrade_button_);
  }
  Controller::scene->removeItem(exit_button_);
  Controller::scene->removeItem(planet_info_);
  Controller::scene->removeItem(background_);
  delete exit_button_;
  delete planet_info_;
  delete background_;
}

void PlanetInfoMenu::Upgrade() {
  Controller::GetActivePlanet()->Upgrade();
  planet_info_->SetLevel(Controller::GetActivePlanet()->GetLevel());
}

void PlanetInfoMenu::Exit() { SwitchTo(Controller::MenuType::kPlanet); }

void PlanetInfoMenu::keyEscapePressed() {
  QShortcut* sc = dynamic_cast<QShortcut*>(QObject::sender());
  if (sc && sc->objectName() == "From KeyReleaseEvent") {
    sc->setObjectName("");
    SwitchTo(Controller::MenuType::kPlanet);
  }
}

SettingsMenu::SettingsMenu() {
  scroll_scene_ = new QGraphicsScene();
  scroll_view_ = new ScrollingView(scroll_scene_, Controller::window);
  int h = qApp->screens()[0]->size().height();
  int w = qApp->screens()[0]->size().width();

  scroll_view_->setGeometry(0, 0, w, h);
  scroll_view_->setSceneRect(Controller::view->sceneRect());
  scroll_view_->show();
  scroll_scene_->setBackgroundBrush(Qt::red);

  Controller::MenuType type = Controller::MenuType::kSettings;
  Qt::Key key_esc = Controller::GetKeyHandler()->Get(type, Qt::Key_Escape).key;
  shortcuts_[key_esc] =
      std::make_shared<QShortcut>(key_esc, Controller::window);

  connect(shortcuts_[key_esc].get(), SIGNAL(activated()), this,
          SLOT(keyEscapeReleased()));
}

SettingsMenu::~SettingsMenu() {
  delete (scroll_scene_);
  delete (scroll_view_);
}

void SettingsMenu::SwitchTo(Controller::MenuType menu) {
  if (!Controller::Graph()->HasConnection(Controller::GetMenuType(), menu)) {
    return;
  }
  Controller::SetMenuType(menu);

  if (menu == Controller::MenuType::kPause) {
    Controller::GetPauseMenu()->Show();
    Controller::SetSettingsMenu(nullptr);
  } else if (menu == Controller::MenuType::kMain) {
    Controller::GetMainMenu()->Show();
    Controller::SetSettingsMenu(nullptr);
  }
}

void SettingsMenu::Draw() {}

void SettingsMenu::SetZValue() {}

Controller::MenuType SettingsMenu::GetPrevMenu() { return prev_menu_; }

void SettingsMenu::SetPrevMenu(Controller::MenuType menu) { prev_menu_ = menu; }

void SettingsMenu::keyEscapeReleased() {
  QShortcut* sc = dynamic_cast<QShortcut*>(QObject::sender());
  if (sc && sc->objectName() == "From KeyReleaseEvent") {
    sc->setObjectName("");
    SwitchTo(Controller::GetSettingsMenu()->GetPrevMenu());
  }
}

QShortcut* Menu::GetShortcut(int key) {
  return (shortcuts_[key] == nullptr ? nullptr : shortcuts_[key].get());
}
