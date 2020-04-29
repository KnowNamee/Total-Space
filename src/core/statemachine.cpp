#include "core/statemachine.h"

#include <memory>

#include "mainwindow.h"
#include "menu.h"
#include "menugraph.h"
#include "scene/gamescene.h"
#include "scene/gameview.h"

// -----------------------------------------------------------

int StateMachine::current_state_ = StateMainMenu;
int StateMachine::kMenuCount = 6;

MainMenu* StateMachine::main_menu = nullptr;
PauseMenu* StateMachine::pause_menu = nullptr;
UnitMenu* StateMachine::unit_menu = nullptr;
PlanetMenu* StateMachine::planet_menu = nullptr;
GameMenu* StateMachine::game_menu = nullptr;
GameView* StateMachine::view = nullptr;

Planet* StateMachine::active_planet_ = nullptr;

GameScene* StateMachine::scene = nullptr;
MainWindow* StateMachine::window = nullptr;

std::unique_ptr<MenuGraph> StateMachine::menu_graph_ = nullptr;

// -----------------------------------------------------------

bool StateMachine::SwitchMenu(int menu) {
  switch (State()) {
    case StateGameMenu:
      game_menu->SwitchTo(menu);
      break;
    case StateMainMenu:
      main_menu->SwitchTo(menu);
      break;
    case StatePlanetMenu:
      planet_menu->SwitchTo(menu);
      break;
    case StatePauseMenu:
      pause_menu->SwitchTo(menu);
      break;
    default:
      return false;
  }
  return true;
}

void StateMachine::LoadMenuGraph() {
  QVector<QVector<int>> connections(kMenuCount);

  connections[StateMainMenu] = {StateGameMenu};
  connections[StateGameMenu] = {StatePlanetMenu, StatePauseMenu};
  connections[StatePlanetMenu] = {StateGameMenu};
  connections[StatePauseMenu] = {StateMainMenu, StateGameMenu};

  menu_graph_ = std::make_unique<MenuGraph>(kMenuCount, connections);
}

const MenuGraph* StateMachine::Graph() { return menu_graph_.get(); }

void StateMachine::StartGame() {
  if (State() == StateMainMenu) {
    RemoveMainMenu();
  }
  SetState(StateGameMenu);
  view->SetNewGameSettings();
  scene->NewGame();
}

void StateMachine::EndGame() { scene->Destroy(); }

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

void StateMachine::RemoveMainMenu() {
  delete (main_menu);
  main_menu = nullptr;
}

void StateMachine::RemovePauseMenu() {
  delete (pause_menu);
  pause_menu = nullptr;
  SetState(StateGameMenu);
}

void StateMachine::RemovePlanetMenu() {
  delete (planet_menu);
  active_planet_ = nullptr;
  planet_menu = nullptr;
  SetState(StateGameMenu);
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
