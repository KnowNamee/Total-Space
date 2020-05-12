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
class ShopMenu;
class GameMenu;
class AttackMenu;
class MoveMenu;
class MenuGraph;

class MainWindow;
class GameView;

class Controller {
 public:
  Controller() = delete;

  enum class MenuType {
    kMain,
    kPause,
    kPlanet,
    kShop,
    kAttack,
    kMove,
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
  static ShopMenu* GetShopMenu();
  static AttackMenu* GetAttackMenu();
  static MoveMenu* GetMoveMenu();
  static PauseMenu* GetPauseMenu();
  static PlanetMenu* GetPlanetMenu();
  static GameMenu* GetGameMenu();

  static void SetMainMenu(MainMenu* menu);
  static void SetShopMenu(ShopMenu* menu);
  static void SetAttackMenu(AttackMenu* menu);
  static void SetMoveMenu(MoveMenu* menu);
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
  static ShopMenu* shop_menu_;
  static AttackMenu* attack_menu_;
  static MoveMenu* move_menu_;
  static PauseMenu* pause_menu_;
  static PlanetMenu* planet_menu_;
  static GameMenu* game_menu_;

  static MenuType current_state_;
  static int kMenuCount;
};

#endif  // STATEMACHINE_H
