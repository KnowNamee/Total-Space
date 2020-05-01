#ifndef PLANETSGRAPH_H
#define PLANETSGRAPH_H

#include <QGraphicsItem>
#include <QList>
#include <map>
#include <memory>
#include <set>

#include "graphics/planetgraphics.h"

class PlanetsGraph {
 private:
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

    void Draw(const QPen& pen, double opacity = 0.2);
    bool IsOnScene() const;
    int IsCollides() const;

    bool operator<(const Edge& rhs_edge) const;

   private:
    PlanetGraphics* lhs_planet_;
    PlanetGraphics* rhs_planet_;
    QGraphicsLineItem* edge_ = nullptr;

    int distance_;
    bool is_on_scene_ = false;
  };

 public:
  PlanetsGraph() = delete;
  PlanetsGraph(const QList<QGraphicsItem*>& items);

  void Draw();
  void DrawLine(PlanetGraphics* lhs_planet, PlanetGraphics* rhs_planet,
                int type);
  void AddEdge(PlanetGraphics* lhs_planet, PlanetGraphics* rhs_planet);

 private:
  std::map<PlanetGraphics*, std::set<std::shared_ptr<Edge>>> graph_;
  std::vector<PlanetGraphics*> planets_;

  void ExtractPlanets(const QList<QGraphicsItem*>& items);
  void FormEdges();

  void BuildSpiderWeb();
  void KraskalBuildMST();
  PlanetGraphics* FindSetDSU(std::map<PlanetGraphics*, PlanetGraphics*>& parent,
                             PlanetGraphics* planet);
  void UnionSetsDSU(std::map<PlanetGraphics*, PlanetGraphics*>& parent,
                    PlanetGraphics* lhs_planet, PlanetGraphics* rhs_planet);
};

#endif  // PLANETSGRAPH_H
