#ifndef PLANETGRAPHICS_H
#define PLANETGRAPHICS_H

#include <QGraphicsItem>
#include <QObject>
#include <memory>

class Planet;

class PlanetGraphics : public QObject, public QGraphicsItem {
  Q_OBJECT
 public:
  PlanetGraphics(const std::shared_ptr<Planet>& planet);

 private:
  QRectF boundingRect() const override;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
             QWidget *widget) override;

  const std::shared_ptr<Planet> planet_;
};

#endif  // PLANETGRAPHICS_H
