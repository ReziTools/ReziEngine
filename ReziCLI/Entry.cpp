#include "ReziContext.hpp"
#include "ReziSolver.hpp"
#include "Version.hpp"
#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char **argv) {
  std::string err_msg;
  std::string rezicode_path, rawcode;
  std::cout << "ReziCLI " VERSION_STRING "\n";

  if(argc < 2){
    err_msg = "No REZI file provided.";
  }

  std::ifstream fin(argv[1]);
  if(!fin.is_open())
    err_msg = "File couldn't be opened.";

  std::stringstream buffer;
  buffer << fin.rdbuf();
  rawcode = buffer.str();

  std::cout << rawcode << '\n';

  if(err_msg.empty()){
  }

  if (!err_msg.empty()) {
    std::cerr << err_msg << '\n';
    return -1;
  }
  return 0;
}
