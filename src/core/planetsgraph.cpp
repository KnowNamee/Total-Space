#include "planetsgraph.h"

#include <QDebug>
#include <algorithm>
#include <cmath>
#include <queue>

#include "objects/planet.h"

PlanetsGraph::PlanetsGraph(const QList<QGraphicsItem*>& items) {
  ExtractPlanets(items);
  FormEdges();        // O(n^2)
  KraskalBuildMST();  // O(nlogn)
}

void PlanetsGraph::AddEdge(PlanetGraphics* planet_a, PlanetGraphics* planet_b,
                           bool is_shortest) {
  QPointF acoords = planet_a->GetPlanet()->GetCoordinates();
  QPointF bcoords = planet_b->GetPlanet()->GetCoordinates();
  double xdiff = acoords.x() - bcoords.x();
  double ydiff = acoords.y() - bcoords.y();
  int dist = static_cast<int>(std::sqrt(xdiff * xdiff + ydiff * ydiff));
  graph_[planet_a].insert(Edge(planet_b, dist, is_shortest));
  graph_[planet_b].insert(Edge(planet_a, dist, is_shortest));
}

void PlanetsGraph::KraskalBuildMST() {
  int sets_count = static_cast<int>(planets_.size());
  std::priority_queue<
      std::pair<int, std::pair<PlanetGraphics*, PlanetGraphics*>>>
      mst_queue;

  std::map<PlanetGraphics*, PlanetGraphics*> parent;
  for (auto planet : planets_) {
    parent[planet] = planet;
  }

  for (auto& p : graph_) {
    auto lhs_planet = p.first;
    for (const Edge& edge : p.second) {
      auto rhs_planet = edge.GetPlanetGraphics();
      if (lhs_planet != rhs_planet) {
        int dist = edge.GetDistance();
        mst_queue.push({-dist, {lhs_planet, rhs_planet}});
      }
    }
  }
  graph_.clear();

  while (!mst_queue.empty() && sets_count) {
    auto planet_a = mst_queue.top().second.first;
    auto planet_b = mst_queue.top().second.second;
    mst_queue.pop();

    if (FindSetDSU(parent, planet_a) != FindSetDSU(parent, planet_b)) {
      UnionSetsDSU(parent, planet_a, planet_b);
      AddEdge(planet_a, planet_b, true);
      sets_count--;
    }
  }

  Q_ASSERT(sets_count == 1);
}

PlanetGraphics* PlanetsGraph::FindSetDSU(
    std::map<PlanetGraphics*, PlanetGraphics*>& parent,
    PlanetGraphics* planet) {
  if (planet == parent[planet]) {
    return planet;
  }
  return parent[planet] = FindSetDSU(parent, parent[planet]);
}

void PlanetsGraph::UnionSetsDSU(
    std::map<PlanetGraphics*, PlanetGraphics*>& parent,
    PlanetGraphics* planet_a, PlanetGraphics* planet_b) {
  auto set_a = FindSetDSU(parent, planet_a);
  auto set_b = FindSetDSU(parent, planet_b);
  if (set_a != set_b) {
    parent[set_b] = set_a;
  }
}

void PlanetsGraph::ExtractPlanets(const QList<QGraphicsItem*>& items) {
  for (auto item : items) {
    if (item->type() == PlanetGraphics::Type) {
      planets_.push_back(dynamic_cast<PlanetGraphics*>(item));
    }
  }
}

void PlanetsGraph::FormEdges() {
  for (auto lhs_planet : planets_) {
    for (auto rhs_planet : planets_) {
      AddEdge(lhs_planet, rhs_planet);
    }
  }
}

PlanetsGraph::Edge::Edge(PlanetGraphics* planet, int distance, bool is_shortest)
    : planet_(planet), distance_(distance), is_shortest_(is_shortest) {}

Planet* PlanetsGraph::Edge::GetPlanet() const { return planet_->GetPlanet(); }

PlanetGraphics* PlanetsGraph::Edge::GetPlanetGraphics() const {
  return planet_;
}

int PlanetsGraph::Edge::GetDistance() const { return distance_; }

bool PlanetsGraph::Edge::IsShortest() const { return is_shortest_; }

bool PlanetsGraph::Edge::operator<(const PlanetsGraph::Edge& rhs_edge) const {
  return distance_ < rhs_edge.distance_;
}
