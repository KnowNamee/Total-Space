#include "menu.h"
#include "gamescene.h"
#include "mainwindow.h"

#include <QGraphicsScene>
#include <QGraphicsItem>

Menu::Menu(GameScene *scene) : scene_(scene) {}

Menu::~Menu() {}

void Menu::Draw() {}

MainMenu::MainMenu(MainWindow *window) : Menu(window->GetScene()) {
    connect(this, SIGNAL(btnExitClick()), window, SLOT(exit()));
    connect(this, SIGNAL(btnNewGameClick()), window->GetScene(), SLOT(NewGame()));
    // connect(this, SIGNAL(btnSettingsClick()), ??)
    this->Draw();
}

MainMenu::~MainMenu()
{
    scene_->removeItem(txt_total_space_);
    scene_->removeItem(btn_exit_);
    scene_->removeItem(btn_new_game_);
    // scene_->removeItem(btn_settings);
}

void MainMenu::Draw()
{
    txt_total_space_ = new QGraphicsTextItem();
    btn_exit_ = new QGraphicsTextItem();
    btn_new_game_ = new QGraphicsTextItem();
    // btn_settings_ = new QGraphicsTextItem();

    // тут расстановка кнопок в меню, их масштаб, позиционирование и т.п.
}

void MainMenu::Hide()
{
    txt_total_space_->hide();
    btn_exit_->hide();
    btn_new_game_->hide();
}

void MainMenu::Show()
{
    txt_total_space_->show();
    btn_exit_->show();
    btn_new_game_->show();
}

PauseMenu::PauseMenu(MainWindow *window)
{
    connect(this, SIGNAL(btnBackClick()), )
    this->Draw();
}
