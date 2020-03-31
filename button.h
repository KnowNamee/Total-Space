#ifndef BUTTON_H
#define BUTTON_H

#include <QGraphicsItem>

class Button : public QGraphicsItem
{
    Q_INTERFACES()

 public:
    Button();
    Button(const QImage& img);
    Button(const QString& str);

    enum {
        Type = UserType + 1,
    };

    int type() const override;

    QImage img;
    QString str;

 private:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
};

#endif // BUTTON_H
