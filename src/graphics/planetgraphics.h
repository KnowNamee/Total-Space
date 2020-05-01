#ifndef PLANETGRAPHICS_H
#define PLANETGRAPHICS_H

#include <QGraphicsItem>
#include <QObject>
#include <memory>

#include "util/typeoffset.h"

class Planet;
class GameView;

class PlanetGraphics : public QObject, public QGraphicsItem {
  Q_OBJECT
  Q_INTERFACES(QGraphicsItem)

 public:
  PlanetGraphics(const std::shared_ptr<Planet>& planet, GameView* view);

  int type() const override;
  Planet* GetPlanet();

 private:
  QPixmap* planet_image_;
  QRectF boundingRect() const override;
  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
             QWidget* widget) override;

  const std::shared_ptr<Planet> planet_;
  const GameView* view_;

 public:
  enum {
    Type = UserType + TypeOffset::kPlanet,
  };
};

#endif  // PLANETGRAPHICS_H
