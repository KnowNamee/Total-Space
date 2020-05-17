#include "keyhandler.h"

#include <QDebug>

#include "core/statemachine.h"

using MType = Controller::MenuType;

KeyHandler::KeyHandler() {
  // TODO сделать подгрузку с JSON
  // true - изменяемый параметр
  // false - неизменяемый параметр
  AddToData(MType::kGame, {{Qt::Key_Escape, {"Back", Qt::Key_Escape, false}},
                           {Qt::Key_N, {"Next turn", Qt::Key_N, true}}});

  AddToData(MType::kPlanet, {{Qt::Key_Escape, {"Back", Qt::Key_Escape, false}},
                             {Qt::Key_W, {"Attack / Move", Qt::Key_W, true}},
                             {Qt::Key_A, {"Memes / Shop", Qt::Key_A, true}},
                             {Qt::Key_D, {"Info", Qt::Key_D, true}}});

  AddToData(MType::kSettings,
            {{Qt::Key_Escape, {"Back", Qt::Key_Escape, false}}});

  AddToData(MType::kPlanetInfo,
            {{Qt::Key_Escape, {"Back", Qt::Key_Escape, false}}});

  AddToData(MType::kMove, {{Qt::Key_Escape, {"Back", Qt::Key_Escape, false}}});

  AddToData(MType::kUnit, {{Qt::Key_Escape, {"Back", Qt::Key_Escape, false}}});

  AddToData(MType::kPause, {{Qt::Key_Escape, {"Back", Qt::Key_Escape, false}}});

  AddToData(MType::kAttack,
            {{Qt::Key_Escape, {"Back", Qt::Key_Escape, false}}});

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

const std::map<Qt::Key, KeyHandler::Key> &KeyHandler::GetKeys(MType type) {
  return data_[type];
}

void KeyHandler::UpdateKey(KeyField *field, Qt::Key new_key) {
  for (auto &x : data_[field->GetMenu()]) {
    Key &k = x.second;
    if (k.key == new_key || (k.key == field->GetKey() && !k.can_modify)) {
      field->setText(QKeySequence(field->GetKey()).toString());
      return;
    }
  }
  for (auto &x : data_[field->GetMenu()]) {
    Key &k = x.second;
    if (k.key == field->GetKey()) {
      field->setText(QKeySequence(new_key).toString());
      field->SetKey(new_key);
      k.key = new_key;
      return;
    }
  }
  field->setText("Error");
}

const KeyHandler::Key &KeyHandler::Get(MType type, Qt::Key key) {
  return data_[type][key];
}
