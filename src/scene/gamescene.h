#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QApplication>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QObject>
#include <QScreen>
#include <memory>

#include "graphics/imageitem.h"
#include "objects/bot.h"
#include "util/utility.h"

class Drawer;
class Player;
class PlayerBase;
class Planet;
class Unit;
class PlanetsGraph;

class GameScene : public QGraphicsScene {
  Q_OBJECT
 public:
  GameScene(QObject* parent = nullptr);

  void Destroy();
  void HideAll();
  void ShowAll();
  Player* GetPlayer() const;
  double GetMapSize() const;
  std::map<Planet*, QVector<UnitType>> GetNearestUnits(PlayerBase* player);

  void UpdatePlanetsGraph();

 public slots:
  void NewGame();

 private:
  void SetSceneSettings();
  void GenerateMap();
  double Distance(const QPointF& lhs, const QPointF& rhs);

  ImageItem* background_;
  std::shared_ptr<PlanetsGraph> graph_;
  std::shared_ptr<Drawer> drawer_;

  std::shared_ptr<Player> player_;
  std::shared_ptr<Bot> bot1_;
  std::shared_ptr<Bot> bot2_;

  const int32_t kWidth = qApp->screens()[0]->size().width();
  const int32_t kHeight = qApp->screens()[0]->size().height();
  const double kMapSize = 2.5;
  // TODO
  // Выбрать расстояние, очень сильно влияет 
  const double kMaximalDistance = qApp->screens()[0]->size().width() / 2;
};

#endif  // GAMESCENE_H
