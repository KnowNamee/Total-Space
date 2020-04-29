#include "core/statemachine.h"

#include <memory>

#include "mainwindow.h"
#include "menu.h"
#include "menugraph.h"
#include "scene/gamescene.h"
#include "scene/gameview.h"

// -----------------------------------------------------------

Controller::MenuType Controller::current_state_ = Controller::MenuType::Main;
int Controller::kMenuCount = 6;

MainMenu* Controller::main_menu = nullptr;
PauseMenu* Controller::pause_menu = nullptr;
UnitMenu* Controller::unit_menu = nullptr;
PlanetMenu* Controller::planet_menu = nullptr;
GameMenu* Controller::game_menu = nullptr;
GameView* Controller::view = nullptr;

Planet* Controller::active_planet_ = nullptr;

GameScene* Controller::scene = nullptr;
MainWindow* Controller::window = nullptr;

std::unique_ptr<MenuGraph> Controller::menu_graph_ = nullptr;

// -----------------------------------------------------------

bool Controller::SwitchMenu(MenuType menu) {
  if (current_state_ == MenuType::Game) {
    game_menu->SwitchTo(menu);
    return true;
  } else if (current_state_ == MenuType::Main) {
    main_menu->SwitchTo(menu);
    return true;
  } else if (current_state_ == MenuType::Pause) {
    pause_menu->SwitchTo(menu);
    return true;
  } else if (current_state_ == MenuType::Planet) {
    planet_menu->SwitchTo(menu);
    return true;
  }
  return false;
}

void Controller::LoadMenuGraph() {
  QVector<QVector<MenuType>> connections(kMenuCount);

  connections[int(MenuType::Main)] = {MenuType::Game};
  connections[int(MenuType::Game)] = {MenuType::Planet, MenuType::Pause};
  connections[int(MenuType::Planet)] = {MenuType::Game};
  connections[int(MenuType::Pause)] = {MenuType::Main, MenuType::Game};

  menu_graph_ = std::make_unique<MenuGraph>(kMenuCount, connections);
}

const MenuGraph* Controller::Graph() { return menu_graph_.get(); }

void Controller::StartGame() {
  if (GetMenuType() == MenuType::Main) {
    RemoveMainMenu();
  }
  SetMenuType(MenuType::Game);
  view->SetNewGameSettings();
  scene->NewGame();
}

void Controller::EndGame() { scene->Destroy(); }

void Controller::HideGame() { scene->HideAll(); }

void Controller::ShowGame() { scene->ShowAll(); }

void Controller::DrawMainMenu() {
  if (GetMenuType() == MenuType::Pause) {
    RemovePauseMenu();
    EndGame();
  }
  SetMenuType(MenuType::Main);
  main_menu = new MainMenu();
}

void Controller::DrawPauseMenu() {
  SetMenuType(MenuType::Pause);
  pause_menu = new PauseMenu();
}

void Controller::DrawPlanetMenu() {
  SetMenuType(MenuType::Planet);
  planet_menu = new PlanetMenu();
}

void Controller::DrawUnitMenu() {
  SetMenuType(MenuType::Unit);
  HideGame();
  HidePlanetMenu();
  unit_menu = new UnitMenu();
}

void Controller::RemoveMainMenu() {
  delete (main_menu);
  main_menu = nullptr;
}

void Controller::RemovePauseMenu() {
  delete (pause_menu);
  pause_menu = nullptr;
  SetMenuType(MenuType::Game);
}

void Controller::RemovePlanetMenu() {
  delete (planet_menu);
  active_planet_ = nullptr;
  planet_menu = nullptr;
  SetMenuType(MenuType::Game);
}

void Controller::RemoveUnitMenu() {
  delete (unit_menu);
  unit_menu = nullptr;
  SetMenuType(MenuType::Planet);
}

void Controller::HidePlanetMenu() {
  assert(planet_menu != nullptr);
  planet_menu->Hide();
}

void Controller::SetMenuType(MenuType next_state) {
  current_state_ = next_state;
}

Controller::MenuType Controller::GetMenuType() { return current_state_; }

Planet* Controller::GetActivePlanet() { return active_planet_; }

void Controller::SetActivePlanet(Planet* planet) { active_planet_ = planet; }
