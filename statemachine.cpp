#include "statemachine.h"

#include "menu.h"
#include "gamescene.h"
#include "mainwindow.h"

#include <QDebug>

// -----------------------------------------------------------

int StateMachine::current_state_ = StateMainMenu;

MainMenu* StateMachine::main_menu = nullptr;
PauseMenu* StateMachine::pause_menu = nullptr;
UnitMenu* StateMachine::unit_menu = nullptr;
PlanetMenu* StateMachine::planet_menu = nullptr;

Unit* StateMachine::active_unit_ = nullptr;
Planet* StateMachine::active_planet_ = nullptr;

GameScene* StateMachine::scene = nullptr;

MainWindow* StateMachine::window = nullptr;

// -----------------------------------------------------------

void StateMachine::StartGame()
{
    qDebug() << "Game Started";
    if (State() == StateMainMenu) {
        RemoveMainMenu();
    }
    SetState(StateGame);
    scene->NewGame();
}

void StateMachine::EndGame()
{
    qDebug() << "Game Ended";
    SetState(StateNone);
    scene->Destroy();
    // TODO
}

void StateMachine::HideGame()
{
    qDebug() << "Game Hided";
    scene->HideAll();
}

void StateMachine::ShowGame()
{
    qDebug() << "Game Showed";
    scene->ShowAll();
}

void StateMachine::DrawMainMenu()
{
    if (State() == StatePauseMenu) {
        RemovePauseMenu();
        EndGame();
    }
    qDebug() << "DrawMainMenu";
    SetState(StateMainMenu);
    main_menu = new MainMenu();
}

void StateMachine::DrawPauseMenu()
{
    qDebug() << "DrawPauseMenu";
    SetState(StatePauseMenu);
    pause_menu = new PauseMenu();
}

void StateMachine::DrawPlanetMenu()
{
    qDebug() << "DrawPlanetMenu";
    SetState(StatePlanetMenu);
    planet_menu = new PlanetMenu();
}

void StateMachine::DrawUnitMenu()
{
    SetState(StateUnitMenu);
    HideGame();
    HidePlanetMenu();
    qDebug() << "DrawPauseMenu";
    unit_menu = new UnitMenu();
}

void StateMachine::RemoveMainMenu()
{
    qDebug() << "MainMenuRemoved";
    delete(main_menu);
    main_menu = nullptr;
    SetState(StateNone);
}

void StateMachine::RemovePauseMenu()
{
    qDebug() << "PauseMenuRemoved";
    delete(pause_menu);
    pause_menu = nullptr;
    SetState(StateGame);
}

void StateMachine::RemovePlanetMenu()
{
    qDebug() << "PlanetMenuRemoved";
    delete(planet_menu);
    planet_menu = nullptr;
    SetState(StateGame);
}

void StateMachine::RemoveUnitMenu()
{
    qDebug() << "UnitMenuDeleted";
    delete(unit_menu);
    unit_menu = nullptr;
    SetState(StatePlanetMenu);
}

void StateMachine::HidePlanetMenu()
{
    assert(planet_menu != nullptr);
    qDebug() << "PlanetMenuHided";
    planet_menu->Hide();
}

void StateMachine::SetState(int next_state)
{
    if (next_state == StateGame) {
        qDebug() << "StateGame";
    }
    if (next_state == StateNone) {
        qDebug() << "StateNone";
    }
    if (next_state == StatePauseMenu) {
        qDebug() << "StatePauseMenu";
    }
    if (next_state == StateMainMenu) {
        qDebug() << "StateMainMenu";
    }
    if (next_state == StateUnitMenu) {
        qDebug() << "StateUnitMenu";
    }
    if (next_state == StatePlanetMenu) {
        qDebug() << "StatePlanetMenu";
    }
    current_state_ = next_state;
}

int StateMachine::State()
{
    return current_state_;
}
