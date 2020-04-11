#include "menu.h"

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsView>

#include "gamescene.h"
#include "button.h"
#include "mainwindow.h"
#include "statemachine.h"
#include "planet.h"
#include "gameview.h"

MainMenu::MainMenu() {
    connect(this, SIGNAL(btnExitClick()), StateMachine::window, SLOT(Exit()));
    connect(this, SIGNAL(btnNewGameClick()), StateMachine::window, SLOT(StartGame()));
    this->Draw();
}

MainMenu::~MainMenu() {
    StateMachine::scene->removeItem(txt_total_space_);
    StateMachine::scene->removeItem(btn_exit_);
    StateMachine::scene->removeItem(btn_new_game_);
}

void MainMenu::Draw() {
    txt_total_space_ = new QGraphicsTextItem("Total Space");
    btn_exit_ = new Button("Exit");
    btn_new_game_ = new Button("New game");

    StateMachine::scene->addItem(txt_total_space_);
    StateMachine::scene->addItem(btn_exit_);
    StateMachine::scene->addItem(btn_new_game_);

    QPointF cp = StateMachine::view->sceneRect().center() / 2;

    txt_total_space_->setPos(cp - QPoint(100, 0));
    btn_exit_->setPos(cp - QPoint(200, 0));
    btn_new_game_->setPos(cp - QPoint(300, 0));
}

PauseMenu::PauseMenu() {
    connect(this, SIGNAL(btnBackClick()), StateMachine::window, SLOT(RemovePauseMenu()));
    connect(this, SIGNAL(btnExitClick()), StateMachine::window, SLOT(DrawMainMenu()));
    this->Draw();
}

PauseMenu::~PauseMenu() {
    StateMachine::scene->removeItem(btn_back_);
    StateMachine::scene->removeItem(btn_exit_);
    StateMachine::scene->removeItem(background_rect_);
}

void PauseMenu::Draw() {
    background_rect_ = new QGraphicsRectItem();
    background_rect_->setRect(StateMachine::view->sceneRect());
    background_rect_->setOpacity(0.7);
    background_rect_->setBrush(QColor(Qt::black));

    btn_back_ = new Button("Back");
    btn_exit_ = new Button("Exit");

    StateMachine::scene->addItem(background_rect_);
    StateMachine::scene->addItem(btn_back_);
    StateMachine::scene->addItem(btn_exit_);

    QPointF cp = background_rect_->rect().center() / 2;

    btn_back_->setPos(cp - QPoint(100, 0));
    btn_exit_->setPos(cp - QPoint(0, 100));
}

PlanetMenu::PlanetMenu() {
    connect(this, SIGNAL(btn1Click()), StateMachine::window, SLOT(RemovePlanetMenu()));
    connect(this, SIGNAL(btn2Click()), StateMachine::window, SLOT(RemovePlanetMenu()));
    connect(this, SIGNAL(btn3Click()), StateMachine::window, SLOT(RemovePlanetMenu()));
    this->Draw();
}

PlanetMenu::~PlanetMenu() {
    StateMachine::scene->removeItem(btn1_);
    StateMachine::scene->removeItem(btn2_);
    StateMachine::scene->removeItem(btn3_);
}

void PlanetMenu::Draw() {
    btn1_ = new Button("btn1");
    btn2_ = new Button("btn2");
    btn3_ = new Button("btn3");

    StateMachine::scene->addItem(btn1_);
    StateMachine::scene->addItem(btn2_);
    StateMachine::scene->addItem(btn3_);

    Planet* p = StateMachine::GetActivePlanet();
    radius_ = -(p->Radius() * 1.2);  // TODO определить зависимость множителя от радиуса планеты
    QPointF vec1(0, radius_);
    vec1 = vec1 * QTransform().rotate(-5);
    QPointF vec2(vec1 * QTransform().rotate(60));
    QPointF vec3(vec1 * QTransform().rotate(-60));

    btn1_->setPos(p->Coordinates() + vec1);
    btn2_->setPos(p->Coordinates() + vec2);
    btn3_->setPos(p->Coordinates() + vec3);
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

UnitMenu::UnitMenu() {
    this->Draw();
}

UnitMenu::~UnitMenu() {}

void UnitMenu::Draw() {}
