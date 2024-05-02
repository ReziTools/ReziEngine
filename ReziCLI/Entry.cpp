#include "ReziContext.hpp"
#include "ReziSolver.hpp"
#include "Version.hpp"
#include <iostream>
#include <string>

int main(int argc, char **argv) {
  std::string errMsg;
  std::cout << "ReziCLI " VERSION_STRING "\n";

  if(argc < 2){
    std::cerr << "No REZI file provided.\n";
    return -1;
  }

  ReziContext mainContext;
  mainContext.LoadToml(argv[1], errMsg);

  if(errMsg.empty()){
    ReziSolver::SolveT(mainContext, errMsg);
  }

  if (!errMsg.empty()) {
    std::cerr << errMsg << '\n';
    return -1;
  }
  return 0;
}
