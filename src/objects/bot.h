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
  Bot(int type, std::shared_ptr<Planet> planet);

  int Type();

  enum {
    kRed,
    kGreen,
  };

 private:
  int type_;
};

#endif  // BOT_H
