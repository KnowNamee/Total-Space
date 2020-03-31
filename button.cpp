#include "button.h"

#include <QPainter>

Button::Button(const QImage &img) : img(img) {}

Button::Button(const QString &str) : str(str) {}

int Button::type() const {
    return Type;
}

QRectF Button::boundingRect() const
{
    // TODO возврат относительно размера png
     return QRectF(pos(), QSize(70, 15));
    /* return QRectF(pos(), QSize(
                        img.width() / 2,
                        img.height() / 2
                      )); */
}

void Button::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    QRectF rect = boundingRect();
    // painter->drawImage(rect, img);
    painter->fillRect(rect, QColor(Qt::red));
    painter->drawText(rect, str);
    Q_UNUSED(option)
    Q_UNUSED(widget)
}
