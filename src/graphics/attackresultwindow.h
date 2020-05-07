#ifndef ATTACKRESULTWINDOW_H
#define ATTACKRESULTWINDOW_H

#include <util/utility.h>

#include <QGraphicsItem>

class AttackResultWindow : public QGraphicsItem {
 public:
  AttackResultWindow(const std::map<UnitType, int32_t>& units_to_quantity,
                     const QString& result, const QString& caption,
                     int32_t width, int32_t height);

 private:
  QRectF boundingRect() const override;
  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
             QWidget* widget) override;

  std::map<UnitType, int32_t> units_to_quantity_;
  const QString result_;
  const QString caption_;
  int32_t width_;
  int32_t height_;

  const int32_t kUnitsTextLeft = width_ / 8;
  const int32_t kUnitsTextRight = width_ - width_ / 8;
  const int32_t kStepX = (kUnitsTextRight - kUnitsTextLeft) / 3;
  const int32_t kUnitsTextTop = height_ / 3;
  const int32_t kUnitsTextBottom = 2 * height_ / 3;
  const int32_t kStepY = (kUnitsTextBottom - kUnitsTextTop) / 3;
};

#endif  // ATTACKRESULTWINDOW_H
