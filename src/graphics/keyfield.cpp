#include "keyfield.h"

#include <QKeySequence>

KeyField::KeyField(QWidget* parent) : QLineEdit(parent) {}

KeyField::KeyField(const QString& str, QWidget* parent)
    : QLineEdit(str, parent) {}

KeyField::KeyField(Qt::Key key, Controller::MenuType type)
    : QLineEdit(QKeySequence(key).toString()), key_(key), type_(type) {}

Qt::Key KeyField::GetKey() { return key_; }

Controller::MenuType KeyField::GetMenu() { return type_; }

void KeyField::SetKey(Qt::Key key) { key_ = key; }

void KeyField::SetMenu(Controller::MenuType type) { type_ = type; }

void KeyField::mouseReleaseEvent(QMouseEvent* event) {
  emit clicked();
  Q_UNUSED(event)
}
