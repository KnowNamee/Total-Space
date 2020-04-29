#ifndef MENUGRAPH_H
#define MENUGRAPH_H

#include <QDebug>
#include <QMap>
#include <QSet>
#include <QVector>

class MenuGraph {
 public:
  MenuGraph(int node_count, const QVector<QVector<int>>& node_connections);
  ~MenuGraph() { qDebug() << "MenuGraph destroyed"; }

  bool HasConnection(int lhs_node, int rhs_node) const;

 private:
  QMap<int, QSet<int>> graph_;

  void MakeConnection(int node, const QVector<int>& nodes);
};

#endif  // MENUGRAPH_H
