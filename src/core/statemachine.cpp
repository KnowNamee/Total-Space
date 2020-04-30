#include "core/statemachine.h"

#include "mainwindow.h"
#include "menu.h"
#include "objects/planet.h"
#include "objects/player.h"
#include "scene/gamescene.h"
#include "scene/gameview.h"

// -----------------------------------------------------------

int StateMachine::current_state_ = StateMainMenu;

MainMenu* StateMachine::main_menu = nullptr;
PauseMenu* StateMachine::pause_menu = nullptr;
UnitMenu* StateMachine::unit_menu = nullptr;
PlanetMenu* StateMachine::planet_menu = nullptr;
AttackMenu* StateMachine::attack_menu = nullptr;
GameView* StateMachine::view = nullptr;

Planet* StateMachine::active_planet_ = nullptr;

GameScene* StateMachine::scene = nullptr;
MainWindow* StateMachine::window = nullptr;

// -----------------------------------------------------------

void StateMachine::StartGame() {
  if (State() == StateMainMenu) {
    RemoveMainMenu();
  }
  SetState(StateGame);
  view->SetNewGameSettings();
  scene->NewGame();
}

void StateMachine::EndGame() {
  SetState(StateNone);
  scene->Destroy();
}

void StateMachine::HideGame() { scene->HideAll(); }

void StateMachine::ShowGame() { scene->ShowAll(); }

void StateMachine::DrawMainMenu() {
  if (State() == StatePauseMenu) {
    RemovePauseMenu();
    EndGame();
  }
  SetState(StateMainMenu);
  main_menu = new MainMenu();
}

void StateMachine::DrawPauseMenu() {
  SetState(StatePauseMenu);
  pause_menu = new PauseMenu();
}

void StateMachine::DrawPlanetMenu() {
  SetState(StatePlanetMenu);  
  planet_menu = new PlanetMenu();
}

void StateMachine::DrawUnitMenu() {
  SetState(StateUnitMenu);
  HideGame();
  HidePlanetMenu();
  unit_menu = new UnitMenu();
}

void StateMachine::DrawAttackMenu() {
  // TODO
  // Отрисовка меню атаки, возможно создание отдельного класса для этого
  SetState(StateAttackMenu);
  attack_menu = new AttackMenu();
}

void StateMachine::RemoveMainMenu() {
  delete (main_menu);
  main_menu = nullptr;
  SetState(StateNone);
}

void StateMachine::RemovePauseMenu() {
  delete (pause_menu);
  pause_menu = nullptr;
  SetState(StateGame);
}

void StateMachine::RemovePlanetMenu() {
  delete (planet_menu);
  active_planet_ = nullptr;
  planet_menu = nullptr;
  SetState(StateGame);
}

void StateMachine::RemoveUnitMenu() {
  delete (unit_menu);
  unit_menu = nullptr;
  SetState(StatePlanetMenu);
}

void StateMachine::HidePlanetMenu() {
  assert(planet_menu != nullptr);
  planet_menu->Hide();
}

void StateMachine::SetState(int next_state) { current_state_ = next_state; }

int StateMachine::State() { return current_state_; }

Planet* StateMachine::GetActivePlanet() { return active_planet_; }

void StateMachine::SetActivePlanet(Planet* planet) { active_planet_ = planet; }
