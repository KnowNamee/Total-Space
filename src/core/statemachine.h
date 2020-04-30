#ifndef STATEMACHINE_H
#define STATEMACHINE_H

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
class AttackMenu;

class StateMachine {
 public:
  StateMachine() = delete;

  enum {
    StateMainMenu,
    StatePauseMenu,
    StatePlanetMenu,
    StateUnitMenu,
    StateAttackMenu,
    StateGame,
    StateNone,
  };

  static void StartGame();
  static void EndGame();
  static void HideGame();
  static void ShowGame();

  static void DrawMainMenu();
  static void DrawPauseMenu();
  static void DrawPlanetMenu();
  static void DrawUnitMenu();
  static void DrawAttackMenu();

  static void RemoveMainMenu();
  static void RemovePauseMenu();
  static void RemovePlanetMenu();
  static void RemoveUnitMenu();

  static void HidePlanetMenu();

  static void ShowPlanetMenu();

  static void SetState(int next_state);
  static int State();

  static Planet* GetActivePlanet();
  static void SetActivePlanet(Planet* planet);

  static MainMenu* main_menu;
  static PauseMenu* pause_menu;
  static PlanetMenu* planet_menu;
  static UnitMenu* unit_menu;
  static AttackMenu* attack_menu;
  static GameScene* scene;
  static GameView* view;
  static MainWindow* window;

 private:
  static Planet* active_planet_;

  static int current_state_;
};

#endif  // STATEMACHINE_H
