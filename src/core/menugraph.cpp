#include "menugraph.h"

MenuGraph::MenuGraph(
    int node_count,
    const QVector<QVector<Controller::MenuType>>& node_connections) {
  for (int node = 0; node < node_count; node++) {
    MakeConnection(node, node_connections[node]);
  }
}

bool MenuGraph::HasConnection(Controller::MenuType lhs_node,
                              Controller::MenuType rhs_node) const {
  return graph_[static_cast<int>(lhs_node)].contains(
      static_cast<int>(rhs_node));
}

void MenuGraph::MakeConnection(int node,
                               const QVector<Controller::MenuType>& nodes) {
  if (!graph_.contains(node)) {
    graph_[node] = QSet<int>();
  }
  for (auto node_to_add : nodes) {
    graph_[node].insert(static_cast<int>(node_to_add));
  }
}
