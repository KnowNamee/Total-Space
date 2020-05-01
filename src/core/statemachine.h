#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <QVector>
#include <memory>

class GameScene;
class GameView;

class Unit;
class PlanetGraphics;
class Planet;

class PlanetMenu;
class MainMenu;
class PauseMenu;
class UnitMenu;
class GameMenu;
class MenuGraph;

class MainWindow;
class GameView;
class AttackMenu;

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
  static void SwitchMenu(MenuType menu);
  static void SetMenuType(MenuType type);
  static MenuType GetMenuType();
  static void Destroy();

  static void LoadMenuGraph();
  static const MenuGraph* Graph();

  static Planet* GetActivePlanet();
  static void SetActivePlanet(Planet* planet);

  static MainMenu* GetMainMenu();
  static UnitMenu* GetUnitMenu();
  static PauseMenu* GetPauseMenu();
  static PlanetMenu* GetPlanetMenu();
  static GameMenu* GetGameMenu();

  static void SetMainMenu(MainMenu* menu);
  static void SetUnitMenu(UnitMenu* menu);
  static void SetPauseMenu(PauseMenu* menu);
  static void SetPlanetMenu(PlanetMenu* menu);
  static void SetGameMenu(GameMenu* menu);

  static GameScene* scene;
  static GameView* view;
  static MainWindow* window;

 private:
  static Planet* active_planet_;

  static std::unique_ptr<MenuGraph> menu_graph_;

  static MainMenu* main_menu_;
  static UnitMenu* unit_menu_;
  static PauseMenu* pause_menu_;
  static PlanetMenu* planet_menu_;
  static GameMenu* game_menu_;

  static MenuType current_state_;
  static int kMenuCount;
};

#endif  // STATEMACHINE_H
