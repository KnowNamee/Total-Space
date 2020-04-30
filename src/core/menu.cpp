#include "menu.h"

#include <QApplication>
#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QScreen>

#include "core/statemachine.h"
#include "data/loader.h"
#include "graphics/imageitem.h"
#include "mainwindow.h"
#include "menugraph.h"
#include "objects/planet.h"
#include "objects/player.h"
#include "scene/gamescene.h"
#include "scene/gameview.h"

MainMenu::MainMenu() {
  connect(this, SIGNAL(btnExitClick()), Controller::window, SLOT(Exit()));
  this->Draw();
}

MainMenu::~MainMenu() {
  Controller::scene->removeItem(txt_total_space_);
  Controller::scene->removeItem(btn_exit_);
  Controller::scene->removeItem(btn_new_game_);
}

void MainMenu::Draw() {
  int32_t width = qApp->screens()[0]->size().width();
  int32_t height = qApp->screens()[0]->size().height();

  GameView* view = Controller::view;

  btn_exit_ =
      new ImageItem(Loader::GetButtonImage(ButtonsEnum::kExitButton),
                    static_cast<int>(width / (5 * view->matrix().m11())),
                    static_cast<int>(height / (12 * view->matrix().m11())));
  btn_new_game_ =
      new ImageItem(Loader::GetButtonImage(ButtonsEnum::kNewGameButton),
                    static_cast<int>(width / (5 * view->matrix().m11())),
                    static_cast<int>(height / (12 * view->matrix().m11())));
  txt_total_space_ =
      new ImageItem(Loader::GetButtonImage(ButtonsEnum::kBackground),
                    static_cast<int>(width / view->matrix().m11()),
                    static_cast<int>(height / view->matrix().m11()));

  Controller::scene->addItem(txt_total_space_);
  Controller::scene->addItem(btn_exit_);
  Controller::scene->addItem(btn_new_game_);

  QPointF cp = Controller::view->sceneRect().center() / 2;

  btn_new_game_->setPos(cp - QPoint(0, height / 49) / view->matrix().m11());
  btn_exit_->setPos(cp + QPoint(0, height / 28) / view->matrix().m11());
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

void PauseMenu::Draw() {
  int32_t width = qApp->screens()[0]->size().width();
  int32_t height = qApp->screens()[0]->size().height();

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
                    static_cast<int>(width / (5 * view->matrix().m11())),
                    static_cast<int>(height / (12 * view->matrix().m11())));
  btn_exit_ =
      new ImageItem(Loader::GetButtonImage(ButtonsEnum::kToMenuButton),
                    static_cast<int>(width / (5 * view->matrix().m11())),
                    static_cast<int>(height / (12 * view->matrix().m11())));

  Controller::scene->addItem(background_rect_);
  Controller::scene->addItem(btn_back_);
  Controller::scene->addItem(btn_exit_);

  btn_back_->setPos(view->sceneRect().center() / 2 -
                    QPoint(0, static_cast<int>(height / 14)) /
                        view->matrix().m11());
  btn_exit_->setPos(btn_back_->pos() +
                    QPoint(0, static_cast<int>(height / 18)) /
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

// PlanetMenu::PlanetMenu() {
//  int32_t width = qApp->screens()[0]->size().width();
//  int32_t height = qApp->screens()[0]->size().height();
//  if (Controller::GetActivePlanet()->GetOwner() ==
//      dynamic_cast<PlayerBase*>(Controller::scene->GetPlayer())) {
//    // Building menu
//    connect(this, SIGNAL(btn1Click()), Controller::window,
//            SLOT(RemovePlanetMenu()));
//    // Unit menu
//    connect(this, SIGNAL(btn2Click()), Controller::window,
//            SLOT(RemovePlanetMenu()));
//    // Exit menu
//    connect(this, SIGNAL(btn3Click()), Controller::window,
//            SLOT(RemovePlanetMenu()));
//    btn1_ = new ImageItem(Loader::GetButtonImage(ButtonsEnum::kSimpleButton),
//                          width / 12, height / 15);
//    btn2_ = new ImageItem(Loader::GetButtonImage(ButtonsEnum::kSimpleButton),
//                          width / 12, height / 15);
//    btn3_ = new ImageItem(Loader::GetButtonImage(ButtonsEnum::kSimpleButton),
//                          width / 12, height / 15);
//    this->Draw();
//  } else {
//    // TODO
//    // Attack menu
//    connect(this, SIGNAL(btn1Click()), Controller::window,
//            SLOT(DrawAttackMenu()));
//    // Exit menu
//    connect(this, SIGNAL(btn2Click()), Controller::window,
//            SLOT(RemovePlanetMenu()));
//    // Planet info
//    connect(this, SIGNAL(btn3Click()), Controller::window,
//            SLOT(RemovePlanetMenu()));
//    btn1_ = new ImageItem(Loader::GetButtonImage(ButtonsEnum::kSimpleButton),
//                          width / 12, height / 15);
//    btn2_ = new ImageItem(Loader::GetButtonImage(ButtonsEnum::kSimpleButton),
//                          width / 12, height / 15);
//    // Planet info
//    btn3_ = new ImageItem(Loader::GetButtonImage(ButtonsEnum::kSimpleButton),
//                          width / 12, height / 15);
//    this->Draw();
//  }
//}

PlanetMenu::PlanetMenu() { this->Draw(); }

PlanetMenu::~PlanetMenu() {
  Controller::scene->removeItem(btn1_);
  Controller::scene->removeItem(btn2_);
  Controller::scene->removeItem(btn3_);
}

void PlanetMenu::Draw() {
  int32_t width = qApp->screens()[0]->size().width();
  int32_t height = qApp->screens()[0]->size().height();

  btn1_ = new ImageItem(Loader::GetButtonImage(ButtonsEnum::kSimpleButton),
                        width / 12, height / 15);
  btn2_ = new ImageItem(Loader::GetButtonImage(ButtonsEnum::kSimpleButton),
                        width / 12, height / 15);
  btn3_ = new ImageItem(Loader::GetButtonImage(ButtonsEnum::kSimpleButton),
                        width / 12, height / 15);

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

void PlanetMenu::SwitchTo(Controller::MenuType menu) {
  if (!Controller::Graph()->HasConnection(Controller::GetMenuType(), menu)) {
    return;
  }

  if (menu == Controller::MenuType::kGame) {
    Controller::SetPlanetMenu(nullptr);
    Controller::SetMenuType(Controller::MenuType::kGame);
  }
}

UnitMenu::UnitMenu() { this->Draw(); }

UnitMenu::~UnitMenu() {}

void UnitMenu::Draw() {}

GameMenu::GameMenu() { this->Draw(); }

GameMenu::~GameMenu() { Controller::scene->Destroy(); }

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

Menu::Menu() {}

void Menu::Draw() {}

void Menu::SwitchTo(Controller::MenuType menu) { Q_UNUSED(menu) }
