#include "planetsgraph.h"

#include <QBrush>
#include <QDebug>
#include <QGraphicsLineItem>
#include <QLinearGradient>
#include <algorithm>
#include <cmath>
#include <queue>

#include "core/statemachine.h"
#include "objects/bot.h"
#include "objects/planet.h"
#include "scene/gamescene.h"
#include "scene/gameview.h"

QString PlanetsGraph::Pen::kDefaultColor = "#D6CFCF";  // gray
int PlanetsGraph::Pen::kDefaultWidth = 7;
Qt::PenStyle PlanetsGraph::Pen::kDefaultStyle = Qt::DashLine;
Qt::PenCapStyle PlanetsGraph::Pen::kDefaultCapStyle = Qt::RoundCap;
double PlanetsGraph::Pen::kActiveOpacity = 0.8;
double PlanetsGraph::Pen::kDefaultOpacity = 0.4;

PlanetsGraph::PlanetsGraph(const QList<QGraphicsItem*>& items) {
  ExtractPlanets(items);
  FormEdges();  // O(n^2) algorithm
  BuildSpiderWeb();
}

void PlanetsGraph::Draw() {
  for (const std::pair<PlanetGraphics*, std::set<std::shared_ptr<Edge>>>&
           graph_pair : graph_) {
    const std::set<std::shared_ptr<Edge>>& edges = graph_pair.second;
    for (const std::shared_ptr<Edge>& edge : edges) {
      if (!edge->IsOnScene()) {
        edge->Draw(Pen::GetDefault());
      }
    }
  }
}

void PlanetsGraph::Update() {
  for (const std::pair<PlanetGraphics*, std::set<std::shared_ptr<Edge>>>&
           graph_pair : graph_) {
    const std::set<std::shared_ptr<Edge>>& edges = graph_pair.second;
    for (const std::shared_ptr<Edge>& edge : edges) {
      edge->Update();
    }
  }
}

Planet* PlanetsGraph::GetBotPlanet() {
  std::map<PlanetGraphics*, std::vector<int>> data;
  for (PlanetGraphics* planet : planets_) {
    if (planet->GetPlanet()->GetOwner()) {
      std::map<PlanetGraphics*, int> dist = DistanceBFS(planet);
      for (const std::pair<PlanetGraphics*, int>& dist_pair : dist) {
        PlanetGraphics* curr_planet = dist_pair.first;
        int distance = dist_pair.second;
        data[curr_planet].push_back(distance);
      }
    }
  }

  for (int i = 4; i >= 1; --i) {
    Planet* at_dist = FindPlanetAtDistanceGE(i, data);
    if (at_dist) {
      return at_dist;
    }
  }
  return nullptr;
}

QVector<Planet*> PlanetsGraph::GetConnectedPlanets(
    PlanetGraphics* planet_grapics) const {
  QVector<Planet*> planets;
  Planet* this_planet = planet_grapics->GetPlanet();
  for (const auto& edge : graph_.at(planet_grapics)) {
    Planet* left_planet = edge->GetLeftPlanet();
    Planet* right_planet = edge->GetRightPlanet();
    if (left_planet == this_planet) {
      planets.push_back(right_planet);
      continue;
    }
      planets.push_back(left_planet);
  }
  return planets;
}

void PlanetsGraph::AddEdge(PlanetGraphics* lhs_planet,
                           PlanetGraphics* rhs_planet) {
  QPointF ac = lhs_planet->GetPlanet()->GetCoordinates();
  QPointF bc = rhs_planet->GetPlanet()->GetCoordinates();
  QPointF diff = ac - bc;
  int dist =
      static_cast<int>(std::sqrt(diff.x() * diff.x() + diff.y() * diff.y()));
  std::shared_ptr<Edge> edge =
      std::make_shared<Edge>(lhs_planet, rhs_planet, dist);
  graph_[lhs_planet].insert(edge);
  graph_[rhs_planet].insert(edge);
}

void PlanetsGraph::ExtractPlanets(const QList<QGraphicsItem*>& items) {
  for (QGraphicsItem* item : items) {
    if (item->type() == PlanetGraphics::Type) {
      planets_.push_back(dynamic_cast<PlanetGraphics*>(item));
    }
  }
}

void PlanetsGraph::FormEdges() {
  for (PlanetGraphics* lhs_planet : planets_) {
    for (PlanetGraphics* rhs_planet : planets_) {
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
  std::priority_queue<std::pair<int, std::shared_ptr<Edge>>> all_edges;
  std::priority_queue<std::pair<int, std::shared_ptr<Edge>>> good_edges;

  for (const std::pair<PlanetGraphics*, std::set<std::shared_ptr<Edge>>>&
           graph_pair : graph_) {
    const std::set<std::shared_ptr<Edge>>& edges = graph_pair.second;
    for (const std::shared_ptr<Edge>& edge : edges) {
      if (edge->GetDistance() > 0 && !edge->IsOnScene()) {
        edge->Draw(Pen::GetDefault());
        all_edges.push({edge->GetDistance(), edge});
      }
    }
  }
  graph_.clear();

  while (!all_edges.empty()) {
    std::shared_ptr<Edge> edge = all_edges.top().second;
    if (edge->IsCollides()) {
      Controller::scene->removeItem(edge->GetEdge());
    } else {
      good_edges.push({edge->GetDistance(), edge});
    }
    all_edges.pop();
  }
  std::swap(all_edges, good_edges);

  while (!all_edges.empty()) {
    auto edge = all_edges.top().second;
    all_edges.pop();

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
  edge_->setZValue(ZValues::kEdge);
  edge_->setPen(pen);

  Controller::scene->addItem(edge_);
  lhs_planet_->setZValue(ZValues::kPlanet);
  rhs_planet_->setZValue(ZValues::kPlanet);
}

void PlanetsGraph::Edge::Update() {
  if (is_updated_) {
    is_updated_ = false;
    return;
  }
  is_updated_ = true;  // Чтобы одно ребро не обновлялось 2 раза.

  if (lhs_planet_->GetPlanet() == Controller::GetActivePlanet() ||
      rhs_planet_->GetPlanet() == Controller::GetActivePlanet()) {
    edge_->setOpacity(Pen::kActiveOpacity);
  } else {
    edge_->setOpacity(Pen::kDefaultOpacity);
  }

  edge_->setPen(Pen::Get(this));
}

bool PlanetsGraph::Edge::IsOnScene() const { return is_on_scene_; }

int PlanetsGraph::Edge::IsCollides() const {
  Q_ASSERT(edge_);
  for (QGraphicsItem* item : edge_->collidingItems()) {
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

Planet* PlanetsGraph::FindPlanetAtDistanceGE(
    int needed_dist, const std::map<PlanetGraphics*, std::vector<int>>& data) {
  for (const std::pair<PlanetGraphics*, std::vector<int>>& data_pair : data) {
    PlanetGraphics* planet = data_pair.first;
    const std::vector<int>& dist_vec = data_pair.second;
    bool skip = false;

    for (int dist : dist_vec) {
      if (dist < needed_dist - 1) {
        skip = true;
        break;
      }
    }

    if (!skip) {
      return planet->GetPlanet();
    }
  }
  return nullptr;
}

QPen PlanetsGraph::Pen::Get(PlanetsGraph::Edge* edge) {
  PlayerBase* lhs_owner = edge->GetLeftPlanet()->GetOwner();
  PlayerBase* rhs_owner = edge->GetRightPlanet()->GetOwner();

  QLinearGradient gradient(edge->GetEdge()->line().p1(),
                           edge->GetEdge()->line().p2());
  if (!lhs_owner) {
    gradient.setColorAt(0, kDefaultColor);
  } else {
    gradient.setColorAt(0, lhs_owner->GetColor());
  }
  if (!rhs_owner) {
    gradient.setColorAt(1, kDefaultColor);
  } else {
    gradient.setColorAt(1, rhs_owner->GetColor());
  }

  QBrush brush(gradient);
  return QPen(brush, kDefaultWidth, kDefaultStyle, kDefaultCapStyle);
}

QPen PlanetsGraph::Pen::GetDefault() {
  return QPen(QColor(kDefaultColor), kDefaultWidth, kDefaultStyle,
              kDefaultCapStyle);
}
