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

class GameScene;
class MainWindow;

class Menu {
public:
    Menu() = delete;
    explicit Menu(GameScene* scene);
    virtual ~Menu();

    virtual void Draw();
    virtual void Hide();
    virtual void Show();

    GameScene* scene_;
};

class MainMenu : public Menu, public QObject {
    Q_OBJECT

public:
    MainMenu() = delete;
    explicit MainMenu(MainWindow* window);
    ~MainMenu() override;

    void Draw() override;
    void Hide() override;
    void Show() override;

signals:
    void btnSettingsClick();
    void btnNewGameClick();
    void btnExitClick();
    // add signals here

private:
    QGraphicsTextItem* txt_total_space_;
    QGraphicsTextItem* btn_new_game_;
    QGraphicsTextItem* btn_exit_;
    // QGraphicsTextItem* btn_settings;
    // QGraphicsItem* btn_load_;
    // add buttons here
};

class PauseMenu: public Menu, public QObject {
    Q_OBJECT

public:
    PauseMenu() = delete;
    explicit PauseMenu(MainWindow *window);
    ~PauseMenu() override;

    void Draw() override;
    void Hide() override;
    void Show() override;

signals:
    void btnBackClick();
    void btnExitClick();
    void btnSettingsClick();
    // TODO btnSaveClick();
    // add signals here

private:
    QGraphicsTextItem* btn_exit_;
    QGraphicsTextItem* btn_back_;
    // QGraphicsTextItem* btn_settings_;
    // QGraphicsTextItem* btn_save_;
    // add buttons here
};

#endif // MENU_H
