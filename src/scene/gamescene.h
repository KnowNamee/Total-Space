#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <memory>
#include <QApplication>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QObject>
#include <QScreen>

#include "graphics/imageitem.h"
#include "util/utility.h"

class Drawer;
class Player;
class PlayerBase;
class Planet;
class Unit;

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

 public slots:
  void NewGame();

 private:
  void SetSceneSettings();
  void GenerateMap();
  double Distance(const QPointF& lhs, const QPointF& rhs);

  ImageItem* background_;
  std::shared_ptr<Drawer> drawer_;
  std::shared_ptr<Player> player_;
  std::vector<std::shared_ptr<Planet>> planets_;
  const int32_t kWidth = qApp->screens()[0]->size().width();
  const int32_t kHeight = qApp->screens()[0]->size().height();
  const double kMapSize = 2.5;
  // TODO
  // Выбрать расстояние, очень сильно влияет 
  const double kMaximalDistance = qApp->screens()[0]->size().width() / 2;
};

#endif  // GAMESCENE_H
