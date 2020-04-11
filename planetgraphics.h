#ifndef PLANETGRAPHICS_H
#define PLANETGRAPHICS_H

#include <QGraphicsItem>
#include <QObject>
#include <memory>

class Planet;
class GameView;

class PlanetGraphics : public QObject, public QGraphicsItem {
  Q_OBJECT
 public:
  PlanetGraphics(const std::shared_ptr<Planet>& planet, GameView* view);

 private:
  QRectF boundingRect() const override;
  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
             QWidget* widget) override;

  const std::shared_ptr<Planet> planet_;
  const GameView* view_;
};

#endif  // PLANETGRAPHICS_H
