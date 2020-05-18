#include "statusbar.h"
#include "core/statemachine.h"
#include "scene/gameview.h"
#include "scene/gamescene.h"
#include <QFont>
#include "data/loader.h"
#include "objects/player.h"
#include <QPainter>
#include <QFontDatabase>

StatusBar::StatusBar(int32_t width, int32_t height) : width_(width), height_(height) {}

QRectF StatusBar::boundingRect() const {
    double scale = Controller::view->matrix().m11();
    double width = width_ / scale;
    double height = height_ / scale;

    return  QRectF(-width / 2, -height / 2, width, height);
}

void StatusBar::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);

    int32_t width = static_cast<int32_t>(boundingRect().width());
    int32_t height = static_cast<int32_t>(boundingRect().height());
    QSize field_size(width / kFieldsCount, height);

    QFont font = QFont(QFontDatabase::applicationFontFamilies(Loader::GetFont()).first(),
                  static_cast<int32_t>(30 / Controller::view->matrix().m11()));
    painter->setFont(font);
//    painter->setPen(Qt::green);

    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::blue);
    painter->drawRect(-width / 2, -height / 2, field_size.width(), field_size.height() / 2);
    painter->setPen(QPen(Qt::green, 20));
    painter->drawText(QRectF(-width / 2, -height / 2 + field_size.height() / 2,
                      field_size.width(), field_size.height() / 2), Qt::AlignHCenter,
                      QString::number(Controller::scene->GetPlayer()->GetBatteries()));

    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::green);
    painter->drawRect(-width / 2 + field_size.width(), -height / 2, field_size.width(),
                      field_size.height() / 2);
    painter->setPen(QPen(Qt::green, 20));
    painter->drawText(QRectF(-width / 2 + field_size.width(), -height / 2 + field_size.height() / 2,
                      field_size.width(), field_size.height() / 2), Qt::AlignHCenter,
                      QString::number(Controller::scene->GetPlayer()->GetTools()));

    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::yellow);
    painter->drawRect(-width / 2 + 2 * field_size.width(), - height / 2, field_size.width(),
                      field_size.height() / 2);
    painter->setPen(QPen(Qt::green, 20));
    painter->drawText(QRectF(-width / 2 + 2 * field_size.width(), -height / 2 + field_size.height() / 2,
                      field_size.width(), field_size.height() / 2), Qt::AlignHCenter,
                      QString::number(Controller::scene->GetPlayer()->GetArmyPower()));
}
