#include "menu.h"

#include <QApplication>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QScreen>

#include "scene/gamescene.h"
#include "scene/gameview.h"
#include "graphics/imageitem.h"
#include "data/loader.h"
#include "mainwindow.h"
#include "objects/planet.h"
#include "core/statemachine.h"

MainMenu::MainMenu() {
  connect(this, SIGNAL(btnExitClick()), StateMachine::window, SLOT(Exit()));
  connect(this, SIGNAL(btnNewGameClick()), StateMachine::window,
          SLOT(StartGame()));
  this->Draw();
}

MainMenu::~MainMenu() {
  StateMachine::scene->removeItem(txt_total_space_);
  StateMachine::scene->removeItem(btn_exit_);
  StateMachine::scene->removeItem(btn_new_game_);
}

void MainMenu::Draw() {
  int32_t width = qApp->screens()[0]->size().width();
  int32_t height = qApp->screens()[0]->size().height();

  GameView* view = StateMachine::view;

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

  StateMachine::scene->addItem(txt_total_space_);
  StateMachine::scene->addItem(btn_exit_);
  StateMachine::scene->addItem(btn_new_game_);

  QPointF cp = StateMachine::view->sceneRect().center() / 2;

  btn_new_game_->setPos(cp - QPoint(0, height / 49) / view->matrix().m11());
  btn_exit_->setPos(cp + QPoint(0, height / 28) / view->matrix().m11());
  txt_total_space_->setPos(cp);
}

PauseMenu::PauseMenu() {
  connect(this, SIGNAL(btnBackClick()), StateMachine::window,
          SLOT(RemovePauseMenu()));
  connect(this, SIGNAL(btnExitClick()), StateMachine::window,
          SLOT(DrawMainMenu()));
  this->Draw();
}

PauseMenu::~PauseMenu() {
  StateMachine::scene->removeItem(btn_back_);
  StateMachine::scene->removeItem(btn_exit_);
  StateMachine::scene->removeItem(background_rect_);
}

void PauseMenu::Draw() {
  int32_t width = qApp->screens()[0]->size().width();
  int32_t height = qApp->screens()[0]->size().height();

  GameView* view = StateMachine::view;
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

  StateMachine::scene->addItem(background_rect_);
  StateMachine::scene->addItem(btn_back_);
  StateMachine::scene->addItem(btn_exit_);

  btn_back_->setPos(view->sceneRect().center() / 2 -
                    QPoint(0, static_cast<int>(height / 14)) /
                        view->matrix().m11());
  btn_exit_->setPos(btn_back_->pos() +
                    QPoint(0, static_cast<int>(height / 18)) /
                        view->matrix().m11());

}

PlanetMenu::PlanetMenu() {
  connect(this, SIGNAL(btn1Click()), StateMachine::window,
          SLOT(RemovePlanetMenu()));
  connect(this, SIGNAL(btn2Click()), StateMachine::window,
          SLOT(RemovePlanetMenu()));
  connect(this, SIGNAL(btn3Click()), StateMachine::window,
          SLOT(RemovePlanetMenu()));
  this->Draw();
}

PlanetMenu::~PlanetMenu() {
  StateMachine::scene->removeItem(btn1_);
  StateMachine::scene->removeItem(btn2_);
  StateMachine::scene->removeItem(btn3_);
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

  StateMachine::scene->addItem(btn1_);
  StateMachine::scene->addItem(btn2_);
  StateMachine::scene->addItem(btn3_);

  // TODO определить зависимость множителя от радиуса планеты
  Planet* p = StateMachine::GetActivePlanet();
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

UnitMenu::UnitMenu() { this->Draw(); }

UnitMenu::~UnitMenu() {}

void UnitMenu::Draw() {}
