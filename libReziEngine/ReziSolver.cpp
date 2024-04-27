#include "ReziSolver.hpp"

void ReziSolver::FakeSolver(ReziContext &context) {
  for (Node &node : context.Nodes) {
    switch (node.type) {
    case NODE_JOINT:
      node.rForce = {1.0f, 1.0f};
      node.rMoment = 1.0f;
      break;
    case NODE_ARTICULATION:
      node.rForce = {1.0f, 1.0f};
      break;
    case NODE_BEARING:
      node.rForce = {0.0f, 1.0f};
      break;
    case NODE_FREE:
      break;
    }
  }
}
