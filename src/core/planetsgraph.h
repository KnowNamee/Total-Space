#ifndef PLANETSGRAPH_H
#define PLANETSGRAPH_H

#include <QGraphicsItem>
#include <QList>
#include <QPen>
#include <map>
#include <memory>
#include <set>

#include "graphics/planetgraphics.h"

class PlanetsGraph {
 private:
  class Edge;

  class Pen {
   public:
    Pen() = delete;
    static QPen Get(Edge* edge);

    static QPen GetDefault();

    static QString kDefaultColor;
    static int32_t kDefaultWidth;

    static Qt::PenStyle kDefaultStyle;
    static Qt::PenCapStyle kDefaultCapStyle;

    static double kActiveOpacity;
    static double kDefaultOpacity;
  };

  class Edge {
   public:
    Edge() = delete;
    Edge(PlanetGraphics* lhs_planet, PlanetGraphics* rhs_planet, int distance);

    Planet* GetLeftPlanet() const;
    Planet* GetRightPlanet() const;
    PlanetGraphics* GetLeftPlanetGraphics() const;
    PlanetGraphics* GetRightPlanetGraphics() const;
    QGraphicsLineItem* GetEdge() const;
    int GetDistance() const;

    void Draw(const QPen& pen, double opacity = Pen::kDefaultOpacity);
    void Update();

    bool IsOnScene() const;
    int IsCollides() const;

    bool operator<(const Edge& rhs_edge) const;

   private:
    PlanetGraphics* lhs_planet_;
    PlanetGraphics* rhs_planet_;
    QGraphicsLineItem* edge_ = nullptr;

    int distance_;
    bool is_on_scene_ = false;
    bool is_updated_ = false;
  };

 public:
  PlanetsGraph() = delete;
  PlanetsGraph(const QList<QGraphicsItem*>& items);

  void Draw();
  void Update();
  Planet* GetBotPlanet();
  QVector<Planet*> GetConnectedPlanets(PlanetGraphics* planet_grapics) const;

 private:
  std::map<PlanetGraphics*, std::set<std::shared_ptr<Edge>>> graph_;
  std::vector<PlanetGraphics*> planets_;

  void ExtractPlanets(const QList<QGraphicsItem*>& items);
  void AddEdge(PlanetGraphics* lhs_planet, PlanetGraphics* rhs_planet);
  void FormEdges();
  Planet* FindPlanetAtDistanceGE(
      int dist, const std::map<PlanetGraphics*, std::vector<int>>& data);

  std::map<PlanetGraphics*, int> DistanceBFS(PlanetGraphics* planet);

  void BuildSpiderWeb();
};

#endif  // PLANETSGRAPH_H
