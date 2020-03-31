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
    // add signals here

 private:
    friend class EventHandler::View;

    QGraphicsTextItem* txt_total_space_;
    Button* btn_new_game_;
    Button* btn_exit_;
    // QGraphicsTextItem* btn_settings;
    // QGraphicsItem* btn_load_;
    // add buttons here
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
    // void btnSettingsClick();
    // TODO btnSaveClick();
    // add signals here

 private:
    friend class EventHandler::View;

    Button* btn_exit_;
    Button* btn_back_;
    QGraphicsRectItem* background_rect_;
    // QGraphicsTextItem* btn_settings_;
    // QGraphicsTextItem* btn_save_;
    // add buttons here
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


 private:
    friend class EventHandler::View;

    QGraphicsItem* btn1;
};

class UnitMenu : public QObject {
    Q_OBJECT

 public:
    UnitMenu();
    ~UnitMenu();

    void Draw();
    void Hide();
    void Show();

 signals:

 private:
    friend class EventHandler::View;
};
#endif // MENU_H
