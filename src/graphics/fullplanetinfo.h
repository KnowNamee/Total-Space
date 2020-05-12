#ifndef FULLPLANETINFO_H
#define FULLPLANETINFO_H

#include <QGraphicsItem>
#include <QString>
#include <cstdint>

#include "util/utility.h"

class QPixmap;
class Planet;

class FullPlanetInfo : public QGraphicsItem {
 public:
  FullPlanetInfo(int32_t width, int32_t height, Planet* planet);
  void SetLevel(int32_t level);

 private:
  QRectF boundingRect() const override;
  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
             QWidget* widget) override;

  std::map<UnitType, UnitData> units_to_data_;
  QString name_;
  QString owner_;
  QPixmap* planet_image_;
  int32_t width_;
  int32_t height_;
  int32_t level_;
  int32_t tools_cost_;
  int32_t batteries_cost_;
  int32_t tools_income_;
  int32_t batteries_income_;
  int32_t nearest_power_;
  int32_t power_;
  bool is_players_;
};

#endif  // FULLPLANETINFO_H
