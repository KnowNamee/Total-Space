#ifndef PLANETINFOGRAPHICS_H
#define PLANETINFOGRAPHICS_H

#include <util/utility.h>

#include <QGraphicsItem>
#include <cstdint>

class QPixmap;

class PlanetInfoGraphics : public QGraphicsItem {
  struct UnitData {
    QPixmap* unit_image;
    QString caption;
    int32_t quantity = 0;
  };

 public:
  PlanetInfoGraphics(QPixmap* planet_image, int32_t width, int32_t height);
  int32_t GetWidth() const;

 private:
  QRectF boundingRect() const override;
  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
             QWidget* widget) override;

  int font_;

  std::map<UnitType, UnitData> units_to_data_;
  QPixmap* planet_image_;
  const QString level_;
  int32_t width_;
  int32_t height_;
};

#endif  // PLANETINFOGRAPHICS_H
