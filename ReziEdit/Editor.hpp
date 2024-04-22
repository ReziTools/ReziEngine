#pragma once
#include "ReziContext.hpp"
#include "gui/Button.hpp"
#include "gui/TextBox.hpp"
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>

enum EditorMode {
  MODE_PAN = 0,
  MODE_ADDNODE,
  MODE_ADDLINE,
  MODE_DELLINE
};

class Editor {
public:
  Editor(int width, int height, bool fullscreen, std::string title);
  void UseContext(ReziContext *_context);
  void Awake(void);
  void Start(void);
  void Update(void);
  void Render(void);
  void RenderGUI(void);
  void RenderNodeProps(void);
  void RenderDebugInfo(void);
  bool IsNodeHovered(size_t index, float radius);
  int GetHoveredNode(float radius);
  ~Editor(void);

private:
  ReziContext *context;
  Font font;
  float guiHeight;
  Camera2D camera;
  TextBox<float> coordsBoxX, coordsBoxY;
  EditorMode editorMode;
  NodeType nodeType;
  unsigned selectionNodesIndex[2];
  Button addNodeButton, nodeTypeButton, moveNodeButton, deleteNodeButton, addLineButton, deleteLineButton;
  int width, height;
  bool fullscreen;
  std::string title;
};
