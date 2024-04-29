#include "ReziSolver.hpp"
#include <Eigen/Dense>
#include <stdexcept>

void ReziSolver::SolveT(ReziContext &context) {
  if (context.GetNodeCount() <= 1)
    throw std::invalid_argument("Context has insufficient nodes.");
  if (!CheckContextDFS(context))
    throw std::invalid_argument("Graph is not connected.");
  if (!CheckContextAlignY(context))
    throw std::invalid_argument("Nodes are unaligned on Y axis.");
  if (!GetContextMomentMatrixCount(context))
    throw std::invalid_argument("Moment equations cannot be generated.");
  Eigen::MatrixXf momentMatrix(GetContextMomentMatrixCount(context) + 1, GetContextMomentMatrixCount(context));
  Eigen::VectorXf reactionVector(GetContextMomentMatrixCount(context));
  Eigen::VectorXf cMomentVector(GetContextMomentMatrixCount(context));
  for (Node &node : context.Nodes) {
    switch (node.type) {
    case NODE_JOINT:
      node.rForce = {0.0f, 1.0f};
      node.rMoment = 1.0f;
      break;
    case NODE_ARTICULATION:
      node.rForce = {0.0f, 1.0f};
      node.rMoment = 0.0f;
      break;
    case NODE_BEARING:
      node.rForce = {0.0f, 1.0f};
      node.rMoment = 0.0f;
      break;
    case NODE_FREE:
      break;
    }
  }
}

template <typename T>
void DFS(const size_t node_index, const std::vector<std::vector<T>> &graph, std::vector<bool> &visited) {
  visited.at(node_index) = true;
  for (size_t i = 0; i < graph.size(); i++) {
    if (!visited.at(i) && graph.at(i).at(node_index))
      DFS(i, graph, visited);
  }
}

bool CheckContextDFS(const ReziContext &context) {
  std::vector<bool> visited(context.GetNodeCount(), false);
  DFS(0, context.Connections, visited);
  for (bool v : visited)
    if (!v)
      return false;
  return true;
}

bool CheckContextAlignX(const ReziContext &context) {
  const Node &first = context.Nodes.at(0);
  for (const Node &node : context.Nodes) {
    if (first.position.x() != node.position.x())
      return false;
  }
  return true;
}

bool CheckContextAlignY(const ReziContext &context) {
  const Node &first = context.Nodes.at(0);
  for (const Node &node : context.Nodes) {
    if (first.position.y() != node.position.y())
      return false;
  }
  return true;
}

size_t GetContextMomentMatrixCount(const ReziContext &context) {
  size_t eq_cnt = 0;
  for (const Node &node : context.Nodes) {
    if (node.type != NODE_FREE)
      eq_cnt++;
  }
  return eq_cnt;
}
