#include "menu.h"
#include "gamescene.h"
#include "button.h"
#include "mainwindow.h"
#include "statemachine.h"

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsView>


MainMenu::MainMenu() {
    connect(this, SIGNAL(btnExitClick()), StateMachine::window, SLOT(Exit()));
    connect(this, SIGNAL(btnNewGameClick()), StateMachine::window, SLOT(StartGame()));
    this->Draw();
}

MainMenu::~MainMenu()
{
    StateMachine::scene->removeItem(txt_total_space_);
    StateMachine::scene->removeItem(btn_exit_);
    StateMachine::scene->removeItem(btn_new_game_);
}

void MainMenu::Draw()
{
    txt_total_space_ = new QGraphicsTextItem("Total Space");
    btn_exit_ = new Button("Exit");
    btn_new_game_ = new Button("New game");

    StateMachine::scene->addItem(txt_total_space_);
    StateMachine::scene->addItem(btn_exit_);
    StateMachine::scene->addItem(btn_new_game_);

    // тут расстановка кнопок в меню, их масштаб, позиционирование и т.п.

    btn_new_game_->setPos(100, 100);
    btn_exit_->setPos(200, 200);
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
    background_rect_ = new QGraphicsRectItem(StateMachine::scene->sceneRect());
    background_rect_->setOpacity(0.7);
    background_rect_->setBrush(QColor(Qt::black));

    btn_back_ = new Button("Back");
    btn_exit_ = new Button("Exit");

    StateMachine::scene->addItem(background_rect_);
    StateMachine::scene->addItem(btn_back_);
    StateMachine::scene->addItem(btn_exit_);

    btn_back_->setPos(100, 100);
    btn_exit_->setPos(200, 200);
    // TODO
}

PlanetMenu::PlanetMenu()
{
    // connect'ы
    this->Draw();
}

PlanetMenu::~PlanetMenu()
{
    StateMachine::scene->removeItem(btn1);
    // и т.д.
}

void PlanetMenu::Draw()
{
    // btn1 = new ...
    // расположение кнопок и т.п.
}

void PlanetMenu::Hide()
{

}

void PlanetMenu::Show()
{

}

UnitMenu::UnitMenu()
{
    // connect
    this->Draw();
}

UnitMenu::~UnitMenu()
{
    // remove item
}

void UnitMenu::Draw()
{
    // new
    // + позиционирование
}
