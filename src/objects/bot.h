#ifndef BOT_H
#define BOT_H

#include <QGraphicsItem>
#include <QList>
#include <memory>

#include "planet.h"
#include "playerbase.h"

class Bot : public PlayerBase {
 public:
  Bot() = delete;
  Bot(int color, std::shared_ptr<Planet> planet);

  int Color();

 private:
  int color_;
};

#endif  // BOT_H
