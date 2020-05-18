#include "graphics/shopplanetinfo.h"

#include <QFontDatabase>
#include <QPainter>

#include "core/statemachine.h"
#include "data/loader.h"

ShopPlanetInfo::ShopPlanetInfo(int32_t width, int32_t height, QString caption,
                               int32_t quantity, QPixmap *image)
    : width_(width),
      height_(height),
      item_image_(image),
      caption_(caption),
      quantity_(quantity),
      font_(Loader::GetFont()) {
  if (quantity_ == 0) {
    hide();
  }
}

const int32_t &ShopPlanetInfo::GetQuant() { return quantity_; }

void ShopPlanetInfo::IncQuant() {
  ++quantity_;
  show();
}

QRectF ShopPlanetInfo::boundingRect() const {
  return QRectF(-width_ / 2, -height_ / 2, width_, height_);
}

void ShopPlanetInfo::paint(QPainter *painter,
                           const QStyleOptionGraphicsItem *option,
                           QWidget *widget) {
  QFont fabulist_general =
      QFont(QFontDatabase::applicationFontFamilies(font_).first(),
            static_cast<int32_t>(25));
  // TODO kscale for font
  painter->setFont(fabulist_general);
  painter->setPen(QColor(Qt::white));

  // Добавление картинки, в список который слева
  if (item_image_ != nullptr) {
    painter->drawPixmap(
        QRect(-width_ / 2 + width_ / 30, -height_ / 2, width_ / 3, width_ / 3),
        *item_image_);
  }

  painter->drawText(
      QRectF(-width_ / 20, -height_ / 3, width_ / 2 + width_ / 30, height_ / 2),
      caption_.toLower());
  painter->drawText(QRectF(0, 0, width_ / 2, height_ / 2),
                    QString::number(quantity_));
  Q_UNUSED(option);
  Q_UNUSED(widget);
}
