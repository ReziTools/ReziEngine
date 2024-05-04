#pragma once
#include "ReziContext.hpp"
#include "gui/Button.hpp"
#include "gui/TextBox.hpp"
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>

enum EditorMode {
  MODE_FREE = 0,
  MODE_ADD,
  MODE_INFO
};

const std::string EditorControls =
    "Editor controls:\n"
    "=================================\n"
    "escape: togle free mode\n"
    "n: toggle node add mode\n"
    "shift + left click: select a node\n"
    "c: connect selected nodes\n"
    "d: disconnect selected nodes\n"
    "delete: delete selected nodes\n"
    "t: solve system using Tsolver\n"
    "s: sort system\n";

class Editor {
public:
  static Editor &GetInstance(int width, int height, int fullscreen, std::string title, ReziContext &context);
  static Editor &GetInstance(void);
  void UseContext(ReziContext &_context);
  void Awake(void);
  void Start(void);
  void Loop(void);
  void Update(void);
  void Render(void);
  void RenderGUI(void);
  void RenderTopBar(void);
  void RenderNodeProps(void);
  void RenderDebugInfo(void);
  std::vector<size_t> GetSelectedNodeIndexes(void);
  bool IsNodeHovered(size_t index, float radius);
  int GetHoveredNode(float radius);
  ~Editor(void);

private:
  static Editor *instance;
  Editor(int width, int height, bool fullscreen, std::string title, ReziContext &context);
  Font font;
  bool lockY;
  float nodeRadius;
  std::string errMsg, statusMsg;
  float guiHeight;
  Camera2D camera;
  EditorMode editorMode;
  char solver;
  bool solved;
  std::vector<bool> selectedNodes;
  float forceViewScale, momentViewScale;
  Button addNodeButton, connectButton, disconnectButton, saveButton, loadButton, lockYButton, nodeTypeButton, helpButton;
  TextBox<float> coordsBoxX, coordsBoxY, forceBoxX, forceBoxY, momentBox;
  int width, height;
  bool fullscreen;
  std::string title;
  ReziContext &context;
};
