#ifndef BOT_H
#define BOT_H

#include <QGraphicsItem>
#include <QList>
#include <memory>

#include "planet.h"
#include "playerbase.h"

class Bot : public PlayerBase {
 public:
  Bot(int type, std::shared_ptr<Planet> planet);
  Bot(int type, const QList<QGraphicsItem*>& items);

  int Type();

  enum {
    kRed,
    kYellow,
  };

 private:
  int type_;
};

#endif  // BOT_H
