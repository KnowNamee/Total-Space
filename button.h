#ifndef BUTTON_H
#define BUTTON_H

#include <QGraphicsItem>

class Button : public QGraphicsItem {
    Q_INTERFACES()

 public:
    Button();
    Button(const QImage& img);
    Button(const QString& str_);

    enum {
        Type = UserType + 1,
    };

    int type() const override;

 private:
    QImage img_;
    QString str_;

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
};

#endif // BUTTON_H
