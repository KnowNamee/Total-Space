/*
 * Общие замечания :
 *
 * - Допустимы расширения некоторых полей QGraphicsTextItem в отдельные классы
 * - Кнопки также можно добавлять свои, как и сигналы
 * - Примером является класс MainMenu
 */

#ifndef MENU_H
#define MENU_H

#include <QGraphicsItem>

#include "eventhandling.h"

class Button;
class GameScene;
class MainWindow;

class MainMenu : public QObject {
    Q_OBJECT

 public:
    MainMenu();
    ~MainMenu();

    void Draw();

 signals:
    void btnSettingsClick();
    void btnNewGameClick();
    void btnExitClick();

 private:
    friend class EventHandler::View;

    QGraphicsTextItem* txt_total_space_;
    Button* btn_new_game_;
    Button* btn_exit_;
};

class PauseMenu : public QObject {
    Q_OBJECT

 public:
    PauseMenu();
    ~PauseMenu();

    void Draw();

 signals:
    void btnBackClick();
    void btnExitClick();

 private:
    friend class EventHandler::View;

    Button* btn_exit_;
    Button* btn_back_;
    QGraphicsRectItem* background_rect_;
};

class PlanetMenu : public QObject {
    Q_OBJECT

 public:
    PlanetMenu();
    ~PlanetMenu();

    void Draw();
    void Hide();
    void Show();

 signals:
    void btn1Click();
    void btn2Click();
    void btn3Click();

 private:
    friend class EventHandler::View;

    Button* btn1_;
    Button* btn2_;
    Button* btn3_;

    double radius_;
};

class UnitMenu : public QObject {
    Q_OBJECT

 public:
    UnitMenu();
    ~UnitMenu();

    void Draw();
    void Hide();
    void Show();

 private:
    friend class EventHandler::View;
};
#endif // MENU_H
