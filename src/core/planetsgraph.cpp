#include "planetsgraph.h"

#include <QDebug>
#include <QGraphicsLineItem>
#include <algorithm>
#include <cmath>
#include <queue>

#include "core/statemachine.h"
#include "objects/planet.h"
#include "scene/gamescene.h"
#include "scene/gameview.h"

PlanetsGraph::PlanetsGraph(const QList<QGraphicsItem*>& items) {
  ExtractPlanets(items);
  FormEdges();  // O(n^2)
  BuildSpiderWeb();
  //  KraskalBuildMST();  // O(nlogn)
}

void PlanetsGraph::Draw() {
  for (auto& p : graph_) {
    for (auto edge : p.second) {
      if (edge->IsOnScene()) {
        continue;
      }
      edge->Draw(QPen(Qt::gray, 10, Qt::DashLine));
    }
  }
}

void PlanetsGraph::AddEdge(PlanetGraphics* lhs_planet,
                           PlanetGraphics* rhs_planet) {
  QPointF acoords = lhs_planet->GetPlanet()->GetCoordinates();
  QPointF bcoords = rhs_planet->GetPlanet()->GetCoordinates();
  double xdiff = acoords.x() - bcoords.x();
  double ydiff = acoords.y() - bcoords.y();
  int dist = static_cast<int>(std::sqrt(xdiff * xdiff + ydiff * ydiff));
  std::shared_ptr<Edge> edge =
      std::make_shared<Edge>(lhs_planet, rhs_planet, dist);
  graph_[lhs_planet].insert(edge);
  graph_[rhs_planet].insert(edge);
}

void PlanetsGraph::KraskalBuildMST() {
  int sets_count = static_cast<int>(planets_.size());
  std::priority_queue<std::pair<int, std::shared_ptr<Edge>>> mst_queue;

  std::map<PlanetGraphics*, PlanetGraphics*> parent;
  for (auto planet : planets_) {
    parent[planet] = planet;
  }

  for (auto& p : graph_) {
    for (auto edge : p.second) {
      if (edge->GetDistance() > 0) {
        mst_queue.push({-edge->GetDistance(), edge});
      }
    }
  }
  std::map<PlanetGraphics*, std::set<std::shared_ptr<Edge>>> graph;
  std::swap(graph, graph_);

  while (!mst_queue.empty() && sets_count > 1) {
    auto lhs_planet = mst_queue.top().second->GetLeftPlanetGraphics();
    auto rhs_planet = mst_queue.top().second->GetRightPlanetGraphics();
    mst_queue.pop();

    if (FindSetDSU(parent, lhs_planet) != FindSetDSU(parent, rhs_planet)) {
      UnionSetsDSU(parent, lhs_planet, rhs_planet);
      AddEdge(lhs_planet, rhs_planet);
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
    PlanetGraphics* lhs_planet, PlanetGraphics* rhs_planet) {
  auto set_a = FindSetDSU(parent, lhs_planet);
  auto set_b = FindSetDSU(parent, rhs_planet);
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

void PlanetsGraph::BuildSpiderWeb() {
  std::priority_queue<std::pair<int, std::shared_ptr<Edge>>> edges;
  std::priority_queue<std::pair<int, std::shared_ptr<Edge>>> good_edges;

  for (auto& p : graph_) {
    for (auto edge : p.second) {
      if (edge->GetDistance() > 0 && !edge->IsOnScene()) {
        edge->Draw(QPen(Qt::gray, 10, Qt::DashLine));
        edges.push({edge->GetDistance(), edge});
      }
    }
  }
  graph_.clear();

  while (!edges.empty()) {
    if (edges.top().second->IsCollides()) {
      Controller::scene->removeItem(edges.top().second->GetEdge());
    } else {
      good_edges.push({edges.top().second->GetDistance(), edges.top().second});
    }
    edges.pop();
  }
  std::swap(edges, good_edges);

  while (!edges.empty()) {
    auto edge = edges.top().second;
    edges.pop();

    graph_[edge->GetLeftPlanetGraphics()].insert(edge);
    graph_[edge->GetRightPlanetGraphics()].insert(edge);
  }
}

PlanetsGraph::Edge::Edge(PlanetGraphics* lhs_planet, PlanetGraphics* rhs_planet,
                         int distance)
    : distance_(distance) {
  lhs_planet_ = std::min(lhs_planet, rhs_planet);
  rhs_planet_ = std::max(lhs_planet, rhs_planet);
}

Planet* PlanetsGraph::Edge::GetLeftPlanet() const {
  return lhs_planet_->GetPlanet();
}
Planet* PlanetsGraph::Edge::GetRightPlanet() const {
  return rhs_planet_->GetPlanet();
}

PlanetGraphics* PlanetsGraph::Edge::GetLeftPlanetGraphics() const {
  return lhs_planet_;
}

PlanetGraphics* PlanetsGraph::Edge::GetRightPlanetGraphics() const {
  return rhs_planet_;
}

int PlanetsGraph::Edge::GetDistance() const { return distance_; }

void PlanetsGraph::Edge::Draw(const QPen& pen, double opacity) {
  is_on_scene_ = true;
  edge_ =
      new QGraphicsLineItem(QLineF(lhs_planet_->GetPlanet()->GetCoordinates() /
                                       Controller::view->matrix().m11(),
                                   rhs_planet_->GetPlanet()->GetCoordinates() /
                                       Controller::view->matrix().m11()));
  edge_->setOpacity(opacity);
  edge_->setPen(pen);

  Controller::scene->addItem(edge_);
  lhs_planet_->setZValue(ZValues::kPlanet);
  rhs_planet_->setZValue(ZValues::kPlanet);
}

bool PlanetsGraph::Edge::IsOnScene() const { return is_on_scene_; }

int PlanetsGraph::Edge::IsCollides() const {
  Q_ASSERT(edge_);
  for (auto item : edge_->collidingItems()) {
    if (item->type() == QGraphicsLineItem::Type) {
      if (!item->collidesWithItem(lhs_planet_) &&
          !item->collidesWithItem(rhs_planet_)) {
        return true;
      }
    } else if (item->type() == PlanetGraphics::Type) {
      if (item != lhs_planet_ && item != rhs_planet_) {
        return true;
      }
    }
  }
  return false;
}

QGraphicsLineItem* PlanetsGraph::Edge::GetEdge() const { return edge_; }

bool PlanetsGraph::Edge::operator<(const PlanetsGraph::Edge& rhs_edge) const {
  return distance_ > rhs_edge.distance_;
}
