#include "statemachine.h"

#include "menu.h"
#include "gamescene.h"
#include "mainwindow.h"

int StateMachine::current_state_ = StateMainMenu;

MainMenu *StateMachine::DrawMainMenu(MainWindow *window)
{
    if (State() == StateGame) {
        // window->GetScene()->Destroy();
        // TODO
        // нужно решить, что делаем с объектами не сцене
    }
    SetState(StateMainMenu);
    return new MainMenu(window);
}

PauseMenu *StateMachine::DrawPauseMenu(MainWindow *window)
{
    SetState(StatePauseMenu);
    return new PauseMenu(window);
}

void StateMachine::RemoveMainMenu(MainMenu *menu)
{
    delete(menu);
}

void StateMachine::RemovePauseMenu(PauseMenu *menu)
{
    delete(menu);
}

void StateMachine::HideMainMenu(MainMenu *menu)
{
    menu->Hide();
}

void StateMachine::HidePauseMenu(PauseMenu *menu)
{
    menu->Hide();
}

void StateMachine::ShowMainMenu(MainMenu *menu)
{
    menu->Show();
}

void StateMachine::ShowPauseMenu(PauseMenu *menu)
{
    menu->Show();
}

void StateMachine::SetState(int new_state_)
{
    current_state_ = new_state_;
}

int StateMachine::State()
{
    return current_state_;
}
