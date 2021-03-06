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
  int32_t GetWidth() const;
  int32_t GetHeight() const;
  std::map<Planet*, QVector<UnitType>> GetNearestUnits(PlayerBase* player);
  int32_t GetNearestPower(PlayerBase* player);  
  int32_t GetFontSize(int32_t size) const;
  bool IsPlanetReachable(PlayerBase* player);
  bool IsPlanetOnScene(Planet* planet);
  PlanetsGraph* GetGraph() const;

  void UpdatePlanetsGraph();

 public slots:
  void Next();
  void NewGame();
  void ShowLoseMessage();
  void ShowWinMessage();

 private:
  void SetSceneSettings();
  void GenerateMap();
  void GenerateRandomUnits();
  double Distance(const QPointF& lhs, const QPointF& rhs);

  ImageItem* background_;
  std::shared_ptr<PlanetsGraph> graph_;
  std::shared_ptr<Drawer> drawer_;

  std::shared_ptr<Player> player_;
  std::vector<std::shared_ptr<Planet>> planets_;
  std::shared_ptr<Bot> bot1_;
  std::shared_ptr<Bot> bot2_;

  const int32_t kWidth = qApp->screens()[0]->size().width();
  const int32_t kHeight = qApp->screens()[0]->size().height();
  const uint32_t kMaxInitialPower = 1800;
  const uint32_t kMinInitialPower = 1400;
  const uint32_t kPlayerPower = 1600;
  const double kMapSize = 2.5;
};

#endif  // GAMESCENE_H
