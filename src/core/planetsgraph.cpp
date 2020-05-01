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
      if (!edge->IsOnScene()) {
        edge->Draw(QPen(Qt::gray, 10, Qt::DashLine));
      }
    }
  }
}

std::shared_ptr<Planet> PlanetsGraph::GetBotPlanet() {
  std::vector<int> owners_planets_indexes;
  for (int i = 0; i < planets_.size(); i++) {
    if (planets_[i]->GetPlanet()->GetOwner()) {
      owners_planets_indexes.push_back(i);
    }
  }
  std::map<PlanetGraphics*, std::vector<int>> bfs_result;
  for (int i : owners_planets_indexes) {
    std::map<PlanetGraphics*, int> dist = DistanceBFS(planets_[i]);
    for (auto& p : dist) {
      bfs_result[p.first].push_back(p.second);
    }
  }

  // Find suitable planet ge 5 edges distance
  for (auto& res : bfs_result) {
    bool skip = false;
    for (int dist : res.second) {
      if (dist < 2) {  // real dist >= 3
        skip = true;
        break;
      }
    }
    if (!skip) {
      return res.first->GetPlanet();
    }
  }

  // If no suitable planet found
  for (auto& res : bfs_result) {
    bool skip = false;
    for (int dist : res.second) {
      if (dist < 0) {  // real dist >= 1
        skip = true;
        break;
      }
    }
    if (!skip) {
      return res.first->GetPlanet();
    }
  }

  // Otherwise
  return nullptr;
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

std::map<PlanetGraphics*, int> PlanetsGraph::DistanceBFS(
    PlanetGraphics* planet) {
  std::map<PlanetGraphics*, int> dist;
  std::queue<std::pair<PlanetGraphics*, int>> q;
  q.push({planet, -1});
  dist[planet] = -1;
  while (!q.empty()) {
    auto curr_planet = q.front().first;
    int curr_dist = q.front().second;
    q.pop();
    for (auto edge : graph_[curr_planet]) {
      auto rhs_planet = edge->GetRightPlanetGraphics();
      auto lhs_planet = edge->GetLeftPlanetGraphics();
      if (!dist[rhs_planet]) {
        dist[rhs_planet] = curr_dist + 1;
        q.push({rhs_planet, curr_dist + 1});
      } else if (!dist[lhs_planet]) {
        dist[lhs_planet] = curr_dist + 1;
        q.push({lhs_planet, curr_dist + 1});
      }
    }
  }
  return dist;
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

std::shared_ptr<Planet> PlanetsGraph::Edge::GetLeftPlanet() const {
  return lhs_planet_->GetPlanet();
}
std::shared_ptr<Planet> PlanetsGraph::Edge::GetRightPlanet() const {
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
