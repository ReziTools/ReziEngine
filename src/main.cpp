#include "application.hpp"
#include <cstring>

int main(int argc, char** argv) {
  int width = 800, height = 600, fullscreen = 0;
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
  Application mainApplication(width, height, fullscreen, "REZISTENTAv0.2.5");
  mainApplication.Start();
  return 0;
}
