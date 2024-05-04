#include "ReziSolver.hpp"
#include <Eigen/Dense>

void ReziSolver::SolveT(ReziContext &context, std::string &err) {
  err.clear();

  for (Node &node : context.Nodes) {
    if (node.type == NODE_JOINT) {
      err = "Joints are not allowed for T solver.";
      return;
    }
    if (node.type == NODE_INVALID) {
      err = "System contains invalid nodes.";
      return;
    }
  }

  size_t indCnt = 0;
  for (const Node &node : context.Nodes) {
    if (node.type != NODE_FREE)
      indCnt++;
  }
  if (indCnt < 2) {
    err = "Not enough external joints.";
    return;
  }
  if (indCnt > 2) {
    err = "System has too many external joints, Mohr-Maxwell is not fully implemented.";
    return;
  }

  if (!CheckContextDFS(context)) {
    err = "Graph is not connected.";
    return;
  }
  if (!CheckContextAlignY(context)) {
    err = "Nodes are unaligned on Y axis.";
    return;
  }

  Eigen::MatrixXf distMatrix(indCnt, indCnt);
  Eigen::VectorXf indVector(indCnt);
  Eigen::VectorXf concVector(indCnt);
  std::vector<size_t> indNodeIdxVec;
  auto indNode = [&](size_t index) {
    return &context.Nodes.at(indNodeIdxVec.at(index));
  };
  for (size_t i = 0; i < context.GetNodeCount(); i++) {
    Node &node = context.Nodes.at(i);
    if (node.type == NODE_BEARING || node.type == NODE_ARTICULATION) {
      indNodeIdxVec.push_back(i);
    }
  }

  for (size_t i = 0; i < indCnt; i++) {
    for (size_t j = 0; j < indCnt; j++) {
      if (i == j)
        distMatrix(i, j) = 0.0f;
      else
        distMatrix(i, j) = -(indNode(i)->position.x() - indNode(j)->position.x());
    }
    float cMomentSum = 0.0f;
    for (size_t k = 0; k < context.GetNodeCount(); k++) {
      cMomentSum -= context.Nodes.at(k).cMoment;
      cMomentSum += (indNode(i)->position.x() - context.Nodes.at(k).position.x()) * context.Nodes.at(k).cForce.y();
    }
    concVector(i) = cMomentSum;
  }
  indVector = distMatrix.fullPivLu().solve(concVector);
  for (Node &node : context.Nodes) {
    node.rForce = {0.0f, 0.0f};
    node.rMoment = 0.0f;
  }
  for (size_t i = 0; i < indCnt; i++) {
    indNode(i)->rForce.y() = indVector(i);
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
