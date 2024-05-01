#include "ReziSolver.hpp"
#include <Eigen/Dense>
#include <iostream>

void ReziSolver::SolveT(ReziContext &context, std::string &err) {
  err.clear();
  if (context.GetNodeCount() <= 1) {
    err = "Context has insufficient nodes.";
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
  if (!GetContextMomentMatrixCount(context)) {
    err = "Moment equations cannot be generated.";
    return;
  }
  Eigen::MatrixXf momentMatrix(GetContextMomentMatrixCount(context) + 1, GetContextMomentMatrixCount(context));
  Eigen::VectorXf reactionVector(GetContextMomentMatrixCount(context));
  Eigen::VectorXf cMomentVector(GetContextMomentMatrixCount(context));
  for (Node &node : context.Nodes) {
    switch (node.type) {
    case NODE_JOINT:
      node.rForce = {1.0f, 1.0f};
      node.rMoment = 1.0f;
      break;
    case NODE_ARTICULATION:
      node.rForce = {1.0f, 1.0f};
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

  std::cout << "SFx: ";
  float cFxSum = 0.0f;
  for (size_t i = 0; i < context.GetNodeCount(); i++) {
    const Node &node = context.Nodes.at(i);
    if (node.type == NODE_JOINT || node.type == NODE_ARTICULATION) {
      std::cout << "+H" << i + 1;
    }
    cFxSum -= node.cForce.x();
  }
  std::cout << "=" << cFxSum << '\n';

  std::cout << "SFy: ";
  float cFySum = 0.0f;
  for (size_t i = 0; i < context.GetNodeCount(); i++) {
    const Node &node = context.Nodes.at(i);
    if (node.type != NODE_FREE) {
      std::cout << "+V" << i + 1;
    }
    cFySum -= node.cForce.y();
  }
  std::cout << "=" << cFySum << '\n';

  for (size_t i = 0; i < context.GetNodeCount(); i++) {
    const Node &node = context.Nodes.at(i);
    if (node.type == NODE_FREE)
      continue;
    std::cout << "SM" << i + 1 << ": ";
    // sum of all Reaction*Distance
    for (size_t j = 0; j < context.GetNodeCount(); j++) {
      const Node &node2 = context.Nodes.at(j);
      if (node2.type == NODE_FREE)
        continue;
      std::cout << "+V" << j + 1 << "*" << (node.position - node2.position).norm();
    }
    std::cout << '=';
    // sum of all Concentrated*Distance
    for (size_t j = 0; j < context.GetNodeCount(); j++) {
      const Node &node2 = context.Nodes.at(j);
      if (node2.cForce.y() > LOW_CUTOFF)
        std::cout << '-' << node2.cForce.y() << '*' << (node.position - node2.position).norm();
      /*if(node.cMoment > LOW_CUTOFF)
        std::cout << '+' << node2.cMoment;*/
    }
    std::cout << '\n';
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
