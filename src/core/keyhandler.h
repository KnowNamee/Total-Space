#ifndef KEYHANDLER_H
#define KEYHANDLER_H

#include <QString>
#include <map>

#include "core/statemachine.h"

using MType = Controller::MenuType;

class KeyHandler {
 public:
  struct Key {
    QString description;
    Qt::Key key = Qt::Key_ToggleCallHangup;  // TODO переделать эту кнопку
    bool can_modify = true;
  };

  KeyHandler();

  void AddToData(MType type, const std::map<Qt::Key, Key>& data);
  void UpdateData(MType type, Qt::Key, Qt::Key, const QString& description,
                  bool can_modify = true);
  const std::map<MType, std::map<Qt::Key, Key>>& GetData();

  const Key& Get(MType type, Qt::Key);

 private:
  std::map<MType, std::map<Qt::Key, Key>> data_;
};

#endif  // KEYHANDLER_H
