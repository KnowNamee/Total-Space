#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <QObject>

class GameScene;
class Unit;
class Planet;

class MainMenu;
class PauseMenu;
class PlanetMenu;
class UnitMenu;

class MainWindow;

class StateMachine
{
public:
    StateMachine() = delete;

    enum {
        StateMainMenu,
        StatePauseMenu,
        StatePlanetMenu,
        StateUnitMenu,
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

    static void RemoveMainMenu();
    static void RemovePauseMenu();
    static void RemovePlanetMenu();
    static void RemoveUnitMenu();

    static void HidePlanetMenu();

    static void ShowPlanetMenu();

    static void SetState(int next_state);
    static int State();

    static MainMenu* main_menu;
    static PauseMenu* pause_menu;
    static PlanetMenu* planet_menu;
    static UnitMenu* unit_menu;
    static GameScene* scene;

    static MainWindow* window;

 private:
    static Unit* active_unit_;
    static Planet* active_planet_;

    static int current_state_;
};

#endif // STATEMACHINE_H
