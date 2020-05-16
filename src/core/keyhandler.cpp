#include "keyhandler.h"

#include <QDebug>

#include "core/statemachine.h"

using MType = Controller::MenuType;

KeyHandler::KeyHandler() {
  // TODO сделать подгрузку с JSON
  // true - изменяемый параметр
  // false - неизменяемый параметр
  AddToData(MType::kGame,
            {{Qt::Key_Escape, {"Назад", Qt::Key_Escape, false}},
             {Qt::Key_N, {"Следующий ход", Qt::Key_N, true}},
             {Qt::Key_Enter, {"Открыть меню планеты", Qt::Key_Enter, true}}});

  AddToData(MType::kPlanet,
            {{Qt::Key_Escape, {"Назад", Qt::Key_Escape, false}},
             {Qt::Key_W, {"Атаковать / Переместить", Qt::Key_W, true}},
             {Qt::Key_A, {"Мемы / Магазин", Qt::Key_A, true}},
             {Qt::Key_D, {"Информация", Qt::Key_D, true}}});

  AddToData(MType::kSettings,
            {{Qt::Key_Escape, {"Назад", Qt::Key_Escape, false}}});

  AddToData(MType::kPlanetInfo,
            {{Qt::Key_Escape, {"Назад", Qt::Key_Escape, false}}});

  AddToData(MType::kMove, {{Qt::Key_Escape, {"Назад", Qt::Key_Escape, false}}});

  AddToData(MType::kUnit, {{Qt::Key_Escape, {"Назад", Qt::Key_Escape, false}}});

  AddToData(MType::kPause,
            {{Qt::Key_Escape, {"Назад", Qt::Key_Escape, false}}});

  AddToData(MType::kAttack,
            {{Qt::Key_Escape, {"Назад", Qt::Key_Escape, false}}});

  // TODO добавить до конца
}

void KeyHandler::AddToData(MType type,
                           const std::map<Qt::Key, KeyHandler::Key> &data) {
  data_[type] = data;
}

void KeyHandler::UpdateData(MType type, Qt::Key first_key, Qt::Key secnd_key,
                            const QString &description, bool can_modify) {
  data_[type][first_key] = {description, secnd_key, can_modify};
}

const std::map<MType, std::map<Qt::Key, KeyHandler::Key>>
    &KeyHandler::GetData() {
  return data_;
}

const KeyHandler::Key &KeyHandler::Get(MType type, Qt::Key key) {
  return data_[type][key];
}
