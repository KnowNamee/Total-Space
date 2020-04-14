#include "button.h"

#include <QPainter>

#include "gameview.h"
#include "statemachine.h"

Button::Button(const QImage &img) : img_(img) {}

Button::Button(const QString &str) : str_(str) {}

int Button::type() const { return Type; }

QRectF Button::boundingRect() const {
  // TODO возврат относительно размера png
  if (StateMachine::State() == StateMachine::StatePlanetMenu) {
    return QRectF(pos(), QSize(70, 15));
  }
  return QRectF(pos(), QSize(70, 15) / StateMachine::view->matrix().m11());
}

void Button::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                   QWidget *widget) {
  QRectF rect = boundingRect();
  //  GameView *view = StateMachine::view;

  //  rect.setSize(rect.size() / view->matrix().m11());

  //  QPointF center =
  //      view->mapToScene(QPoint(view->rect().width(), view->rect().height()) /
  //      2);

  // rect.setX(x() / view->matrix().m11());
  // rect.setY(y() / view->matrix().m11());
  //  rect.setY(center.y() - view->rect().height() / (30 *
  //  view->matrix().m11()));

  painter->fillRect(rect, QColor(Qt::red));
  painter->drawText(rect, str_);
  Q_UNUSED(option)
  Q_UNUSED(widget)
}
