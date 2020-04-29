#ifndef MENUGRAPH_H
#define MENUGRAPH_H

#include <QMap>
#include <QSet>
#include <QVector>

#include "core/statemachine.h"

class MenuGraph {
 public:
  MenuGraph(int node_count,
            const QVector<QVector<Controller::MenuType>>& node_connections);

  bool HasConnection(Controller::MenuType lhs_node,
                     Controller::MenuType rhs_node) const;

 private:
  QMap<int, QSet<int>> graph_;

  void MakeConnection(int node, const QVector<Controller::MenuType>& nodes);
};

#endif  // MENUGRAPH_H
