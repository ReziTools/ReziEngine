#include "Editor.hpp"
#include "ReziContext.hpp"
#include "Version.hpp"
#include <cstring>

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
void EditorLoop(void) {
  Editor::GetInstance().Loop();
}
#endif

int main(int argc, char **argv) {
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
  ReziContext mainContext;
  Editor &editor = Editor::GetInstance(width, height, fullscreen, "ReziEdit " VERSION_STRING, mainContext);
  editor.Start();
#if defined(PLATFORM_WEB)
  emscripten_set_main_loop(EditorLoop, 0, 1);
#endif
  return 0;
}
