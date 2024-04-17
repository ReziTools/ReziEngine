#include "application.hpp"
#include <cstring>

int main(int argc, char** argv) {
  int width = 1200, height = 900, fullscreen = 0;
  for (int i = 1; i < argc; ++i) {
    if (!strcmp(argv[i], "-f")) {
      fullscreen = 1;
    } else if (!strcmp(argv[i], "-w") && i + 1 < argc) {
      width = atoi(argv[i + 1]);
      ++i;
    } else if (!strcmp(argv[i], "-h") && i + 1 < argc) {
      height = atoi(argv[i + 1]);
      ++i;
    }
  }
  Application mainApplication(width, height, fullscreen, "Rezistenta v0.2.6");
  mainApplication.Start();
  return 0;
}
