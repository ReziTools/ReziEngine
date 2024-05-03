#include "ReziContext.hpp"
#include "ReziSolver.hpp"
#include "Version.hpp"
#include <iostream>
#include <iomanip>
#include <string>

int main(int argc, char **argv) {
  std::string errMsg;
  char mode;
  std::cout << "ReziCLI " VERSION_STRING "\n";

  if(argc < 2){
    std::cerr << "No REZI file provided.\n";
    return -1;
  }

  if(argc < 3){
    std::cerr << "No solver was specified.\n";
    return -1;
  }

  switch(argv[2][0]){
    case 't':
    case 'T':
      mode = 't';
      break;
    default:
      std::cerr << "Invalid solver.\n";
      return -1;
  }

  ReziContext mainContext;
  mainContext.LoadToml(argv[1], errMsg);

  if(errMsg.empty()){
    switch(mode){
      case 't':
        ReziSolver::SolveT(mainContext, errMsg);
        break;
    }
  }

  if(errMsg.empty()){
    switch(mode){
      case 't':
        float ecVSum = 0.0f;
        for(size_t i=0; i < mainContext.GetNodeCount(); i++){
          const Node& node = mainContext.Nodes.at(i);
          ecVSum += node.rForce.y();
          ecVSum += node.cForce.y();
          if(node.type == NodeType::NODE_FREE) continue;
          std::cout << "V" << i+1 << " = " <<std::setprecision(8)<< node.rForce.y() << '\n';
        }
        std::cout << "ecVsum = " << std::setprecision(8) << ecVSum << '\n';
    }
  }

  if (!errMsg.empty()) {
    std::cerr << errMsg << '\n';
    return -1;
  }
  return 0;
}
