#ifndef PLANETGRAPHICS_H
#define PLANETGRAPHICS_H

#include <QGraphicsItem>
#include <QObject>
#include <memory>

#include "util/typeoffset.h"

class Planet;

class PlanetGraphics : public QObject, public QGraphicsItem {
  Q_OBJECT
  Q_INTERFACES(QGraphicsItem)

 public:
  PlanetGraphics(Planet* planet);

  int type() const override;
  Planet* GetPlanet();
  QPixmap* GetImage();

 private:
  QPixmap* planet_image_;
  QRectF boundingRect() const override;
  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
             QWidget* widget) override;

  Planet* planet_;

 public:
  enum {
    Type = UserType + TypeOffset::Planet,
  };

 protected:
  void mouseReleaseEvent(QGraphicsSceneMouseEvent* mouseEvent) override;
};

#endif  // PLANETGRAPHICS_H
