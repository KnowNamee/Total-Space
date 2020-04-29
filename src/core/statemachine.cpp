#include "core/statemachine.h"

#include <memory>

#include "mainwindow.h"
#include "menu.h"
#include "menugraph.h"
#include "scene/gamescene.h"
#include "scene/gameview.h"

// -----------------------------------------------------------

Controller::MenuType Controller::current_state_ = Controller::MenuType::kMain;
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
  if (current_state_ == MenuType::kGame) {
    game_menu->SwitchTo(menu);
    return true;
  } else if (current_state_ == MenuType::kMain) {
    main_menu->SwitchTo(menu);
    return true;
  } else if (current_state_ == MenuType::kPause) {
    pause_menu->SwitchTo(menu);
    return true;
  } else if (current_state_ == MenuType::kPlanet) {
    planet_menu->SwitchTo(menu);
    return true;
  }
  return false;
}

void Controller::LoadMenuGraph() {
  QVector<QVector<MenuType>> connections(kMenuCount);

  connections[static_cast<int>(MenuType::kMain)] = {MenuType::kGame};
  connections[static_cast<int>(MenuType::kGame)] = {MenuType::kPlanet,
                                                    MenuType::kPause};
  connections[static_cast<int>(MenuType::kPlanet)] = {MenuType::kGame};
  connections[static_cast<int>(MenuType::kPause)] = {MenuType::kMain,
                                                     MenuType::kGame};

  menu_graph_ = std::make_unique<MenuGraph>(kMenuCount, connections);
}

const MenuGraph* Controller::Graph() { return menu_graph_.get(); }

void Controller::SetMenuType(MenuType next_state) {
  current_state_ = next_state;
}

Controller::MenuType Controller::GetMenuType() { return current_state_; }

Planet* Controller::GetActivePlanet() { return active_planet_; }

void Controller::SetActivePlanet(Planet* planet) { active_planet_ = planet; }
