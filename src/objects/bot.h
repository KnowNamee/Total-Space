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
  Bot(Planet* planet, const QString& color);
  virtual ~Bot();

  void Next() override;

 private:
};

#endif  // BOT_H
