#ifndef SHOPPLANETINFO_H
#define SHOPPLANETINFO_H

#include <QGraphicsItem>

class ShopPlanetInfo : public QObject, public QGraphicsItem {
  Q_OBJECT
  Q_INTERFACES(QGraphicsItem)
 public:
  ShopPlanetInfo(int32_t width, int32_t height, QString caption,
                 int32_t quantity, QPixmap* image = nullptr);
  const int32_t& GetQuant();

 public slots:
  void IncQuant();

 private:
  QRectF boundingRect() const override;
  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
             QWidget* widget) override;

  int32_t width_;
  int32_t height_;
  int font_;

  QPixmap* item_image_;
  QString caption_;
  int32_t quantity_;

  const double kPictureZoneCoef = 1. / 3;
};

#endif  // SHOPPLANETINFO_H
