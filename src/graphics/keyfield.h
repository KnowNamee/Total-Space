#ifndef KEYFIELD_H
#define KEYFIELD_H

#include <QLineEdit>

#include "core/statemachine.h"

class KeyField : public QLineEdit {
  Q_OBJECT

 public:
  explicit KeyField(QWidget* parent = nullptr);
  explicit KeyField(const QString& str, QWidget* parent = nullptr);
  KeyField(Qt::Key key, Controller::MenuType type);

  Qt::Key GetKey();
  Controller::MenuType GetMenu();

  void SetKey(Qt::Key key);
  void SetMenu(Controller::MenuType type);

 signals:
  void clicked();

 private:
  Qt::Key key_;
  Controller::MenuType type_;

 protected:
  void mouseReleaseEvent(QMouseEvent* event);
};

#endif  // KEYFIELD_H
