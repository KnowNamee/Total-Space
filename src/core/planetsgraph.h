#ifndef PLANETSGRAPH_H
#define PLANETSGRAPH_H

#include <QGraphicsItem>
#include <QList>
#include <map>
#include <set>

#include "graphics/planetgraphics.h"

class PlanetsGraph {
 private:
  class Edge {
   public:
    Edge() = delete;
    Edge(PlanetGraphics* planet, int distance, bool is_shortest = false);

    Planet* GetPlanet() const;
    PlanetGraphics* GetPlanetGraphics() const;
    int GetDistance() const;
    bool IsShortest() const;

    bool operator<(const Edge& rhs_edge) const;

   private:
    PlanetGraphics* planet_;
    int distance_;
    bool is_shortest_ = false;
  };

 public:
  PlanetsGraph() = delete;
  PlanetsGraph(const QList<QGraphicsItem*>& items);

  void DrawLine(PlanetGraphics* planet_a, PlanetGraphics* planet_b, int type);
  void AddEdge(PlanetGraphics* planet_a, PlanetGraphics* planet_b,
               bool is_shortest = false);

 private:
  std::map<PlanetGraphics*, std::set<Edge>> graph_;
  std::vector<PlanetGraphics*> planets_;

  void ExtractPlanets(const QList<QGraphicsItem*>& items);
  void FormEdges();

  void KraskalBuildMST();
  PlanetGraphics* FindSetDSU(std::map<PlanetGraphics*, PlanetGraphics*>& parent,
                             PlanetGraphics* planet);
  void UnionSetsDSU(std::map<PlanetGraphics*, PlanetGraphics*>& parent,
                    PlanetGraphics* planet_a, PlanetGraphics* planet_b);
};

#endif  // PLANETSGRAPH_H
