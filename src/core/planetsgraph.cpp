#include "planetsgraph.h"

#include <QDebug>
#include <QGraphicsLineItem>
#include <algorithm>
#include <cmath>
#include <queue>

#include "core/statemachine.h"
#include "objects/bot.h"
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

void PlanetsGraph::Update() {
  for (auto& p : graph_) {
    for (auto edge : p.second) {
      edge->Update();
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

  // Find suitable planet ge 4 edges distance
  for (auto& res : bfs_result) {
    bool skip = false;
    for (int dist : res.second) {
      if (dist < 3) {  // real dist >= 4
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
      if (dist < 1) {  // real dist >= 2
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

void PlanetsGraph::Edge::Update() {
  // Отрисовка цвета в зависимости от бота / игрока
  PlayerBase* lhs_owner = lhs_planet_->GetPlanet()->GetOwner();
  PlayerBase* rhs_owner = rhs_planet_->GetPlanet()->GetOwner();

  // TODO переделать / систематизировать выбор прозрачности
  if (lhs_planet_->GetPlanet().get() == Controller::GetActivePlanet() ||
      rhs_planet_->GetPlanet().get() == Controller::GetActivePlanet()) {
    edge_->setOpacity(0.3);
  } else {
    edge_->setOpacity(0.1);
  }

  // TODO полностью переделать систему выбора пера
  // TODO добавить градиентную отрисовку ребра
  QPen yellow_pen(Qt::yellow, 10, Qt::DashLine);
  QPen blue_pen(Qt::blue, 10, Qt::DashLine);
  QPen red_pen(Qt::red, 10, Qt::DashLine);
  QPen green_pen(Qt::green, 10, Qt::DashLine);

  if (lhs_owner && rhs_owner) {
    if (lhs_owner->Type() > rhs_owner->Type()) {
      std::swap(lhs_owner, rhs_owner);
    }
    if (lhs_owner->Type() == PlayerBase::kPlayer) {
      if (rhs_owner->Type() == PlayerBase::kBot) {
        edge_->setPen(yellow_pen);
      } else {
        edge_->setPen(green_pen);
      }
    } else if (lhs_owner->Type() == PlayerBase::kBot) {
      Bot* lhs_bot = reinterpret_cast<Bot*>(lhs_owner);
      Bot* rhs_bot = reinterpret_cast<Bot*>(rhs_owner);
      if (lhs_bot->Color() != rhs_bot->Color()) {
        edge_->setPen(yellow_pen);
      } else {
        if (rhs_bot->Color() == Qt::red) {
          edge_->setPen(red_pen);
        } else {
          edge_->setPen(blue_pen);
        }
      }
    }
  }

  if (lhs_owner && !rhs_owner) {
    std::swap(lhs_owner, rhs_owner);
  }

  if (!lhs_owner && rhs_owner) {
    if (rhs_owner->Type() == PlayerBase::kPlayer) {
      edge_->setPen(green_pen);
    } else {
      Bot* rhs_bot = reinterpret_cast<Bot*>(rhs_owner);
      if (rhs_bot->Color() == Qt::red) {
        edge_->setPen(red_pen);
      } else {
        edge_->setPen(blue_pen);
      }
    }
  }
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
