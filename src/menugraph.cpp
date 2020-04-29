#include "menugraph.h"

MenuGraph::MenuGraph(int node_count,
                     const QVector<QVector<int>>& node_connections) {
  for (int node = 0; node < node_count; node++) {
    MakeConnection(node, node_connections[node]);
  }
}

bool MenuGraph::HasConnection(int lhs_node, int rhs_node) const {
  return graph_[lhs_node].contains(rhs_node);
}

void MenuGraph::MakeConnection(int node, const QVector<int>& nodes) {
  if (!graph_.contains(node)) {
    graph_[node] = QSet<int>();
  }
  for (int node_to_add : nodes) {
    graph_[node].insert(node_to_add);
  }
}
