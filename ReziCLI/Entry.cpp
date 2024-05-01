#include "ReziContext.hpp"
#include "ReziSolver.hpp"
#include "Version.hpp"
#include <iostream>
#include <string>

int main(int argc, char **argv) {
  std::string err_msg;
  std::cout << "ReziCLI " VERSION_STRING "\n";
  ReziContext mainContext;
  mainContext.AddNode({NODE_FREE,
                       {2.0f, 0.0f},
                       {0.0f, 2.0f},
                       0.0f,
                       {0.0f, 0.0f},
                       0.0f});
  mainContext.AddNode({NODE_BEARING,
                       {1.0f, 0.0f},
                       {0.0f, 0.0f},
                       0.0f,
                       {0.0f, 0.0f},
                       0.0f});
  mainContext.AddNode({NODE_ARTICULATION,
                       {0.0f, 0.0f},
                       {0.0f, 0.0f},
                       0.0f,
                       {0.0f, 0.0f},
                       0.0f});
  mainContext.Connections = {
      {0, 1, 0},
      {1, 0, 1},
      {0, 1, 0}};
  ReziSolver::SolveT(mainContext, err_msg);
  if (!err_msg.empty()) {
    std::cerr << err_msg << '\n';
    return -1;
  }
  return 0;
}
