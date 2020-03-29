#ifndef STATEMACHINE_H
#define STATEMACHINE_H

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
    };

    static MainMenu* DrawMainMenu(MainWindow* window);
    static PauseMenu* DrawPauseMenu(MainWindow* window);
    static UnitMenu* DrawPlanetMenu(MainWindow* window, Unit *unit);
    static PlanetMenu* DrawUnitMenu(MainWindow* window, Planet *planet);

    static void RemoveMainMenu(MainMenu* menu);
    static void RemovePauseMenu(PauseMenu *menu);
    static void RemovePlanetMenu(PlanetMenu *menu);
    static void RemoveUnitMenu(UnitMenu *menu);

    static void HideMainMenu(MainMenu* menu);
    static void HidePauseMenu(PauseMenu* menu);
    static void HidePlanetMenu(PlanetMenu* menu);
    static void HideUnitMenu(UnitMenu* menu);

    static void ShowMainMenu(MainMenu* menu);
    static void ShowPauseMenu(PauseMenu* menu);
    static void ShowPlanetMenu(PlanetMenu* menu);
    static void ShowUnitMenu(UnitMenu* menu);

    static void SetState(int new_state_);
    static int State();

private:
    static int current_state_;
};

#endif // STATEMACHINE_H
