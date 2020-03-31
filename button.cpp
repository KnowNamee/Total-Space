#include "button.h"

#include <QPainter>

Button::Button(const QImage &img) : img_(img) {}

Button::Button(const QString &str) : str_(str) {}

int Button::type() const {
    return Type;
}

QRectF Button::boundingRect() const {
    // TODO возврат относительно размера png
     return QRectF(pos(), QSize(70, 15));
    /* return QRectF(pos(), QSize(
                        img.width(),
                        img.height()
                      )); */
}

void Button::paint(QPainter *painter,
                   const QStyleOptionGraphicsItem *option, QWidget *widget) {
    QRectF rect = boundingRect();
    // painter->drawImage(rect, img);
    painter->fillRect(rect, QColor(Qt::red));
    painter->drawText(rect, str_);
    Q_UNUSED(option)
    Q_UNUSED(widget)
}
