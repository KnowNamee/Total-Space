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

QString PlanetsGraph::Pen::kDefaultColor = "#D6CFCF";
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
  for (auto& graph_pair : graph_) {
    auto& edges = graph_pair.second;
    for (auto edge : edges) {
      if (!edge->IsOnScene()) {
        edge->Draw(Pen::GetDefault());
      }
    }
  }
}

void PlanetsGraph::Update() {
  for (auto& graph_pair : graph_) {
    auto& edges = graph_pair.second;
    for (auto edge : edges) {
      edge->Update();
    }
  }
}

std::shared_ptr<Planet> PlanetsGraph::GetBotPlanet() {
  std::map<PlanetGraphics*, std::vector<int>> data;
  for (auto planet : planets_) {
    if (planet->GetPlanet()->GetOwner()) {
      std::map<PlanetGraphics*, int> dist = DistanceBFS(planet);
      for (auto& dist_pair : dist) {
        PlanetGraphics* curr_planet = dist_pair.first;
        int distance = dist_pair.second;

        data[curr_planet].push_back(distance);
      }
    }
  }

  for (int i = 4; i >= 1; --i) {
    std::shared_ptr<Planet> at_dist = FindPlanetAtDistanceGE(i, data);
    if (at_dist) {
      return at_dist;
    }
  }
  return nullptr;
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
  std::priority_queue<std::pair<int, std::shared_ptr<Edge>>> all_edges;
  std::priority_queue<std::pair<int, std::shared_ptr<Edge>>> good_edges;

  for (auto& graph_pair : graph_) {
    const auto& edges = graph_pair.second;
    for (auto edge : edges) {
      if (edge->GetDistance() > 0 && !edge->IsOnScene()) {
        edge->Draw(QPen(Qt::gray, 10, Qt::DashLine));
        all_edges.push({edge->GetDistance(), edge});
      }
    }
  }
  graph_.clear();

  while (!all_edges.empty()) {
    auto edge = all_edges.top().second;
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
  if (is_updated_) {
    is_updated_ = false;
    return;
  }
  is_updated_ = true;  // Чтобы одно ребро не обновлялось 2 раза.

  if (lhs_planet_->GetPlanet().get() == Controller::GetActivePlanet() ||
      rhs_planet_->GetPlanet().get() == Controller::GetActivePlanet()) {
    edge_->setOpacity(Pen::kActiveOpacity);
  } else {
    edge_->setOpacity(Pen::kDefaultOpacity);
  }

  edge_->setPen(Pen::Get(this));
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

std::shared_ptr<Planet> PlanetsGraph::FindPlanetAtDistanceGE(
    int needed_dist, const std::map<PlanetGraphics*, std::vector<int>>& data) {
  for (auto& data_pair : data) {
    PlanetGraphics* planet = data_pair.first;
    const auto& dist_vec = data_pair.second;
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
