#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <memory>

class GameScene;
class Unit;
class PlanetGraphics;
class MainMenu;
class PauseMenu;
class PlanetMenu;
class Planet;
class UnitMenu;
class MainWindow;
class GameView;
class MenuGraph;
class GameMenu;

class Controller {
 public:
  Controller() = delete;

  enum class MenuType {
    kMain,
    kPause,
    kPlanet,
    kUnit,
    kGame,
  };

  static void HideGame();
  static void ShowGame();

  static bool SwitchMenu(MenuType menu);
  static void SetMenuType(MenuType type);
  static MenuType GetMenuType();

  static void LoadMenuGraph();
  static const MenuGraph* Graph();

  static Planet* GetActivePlanet();
  static void SetActivePlanet(Planet* planet);

  static MainMenu* main_menu;
  static PauseMenu* pause_menu;
  static PlanetMenu* planet_menu;
  static UnitMenu* unit_menu;
  static GameScene* scene;
  static GameView* view;
  static MainWindow* window;
  static GameMenu* game_menu;

 private:
  static Planet* active_planet_;
  static std::unique_ptr<MenuGraph> menu_graph_;

  static MenuType current_state_;
  static int kMenuCount;
};

#endif  // STATEMACHINE_H
