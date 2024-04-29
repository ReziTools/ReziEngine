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
  static Editor &GetInstance(int width, int height, int fullscreen, const char *title);
  static Editor &GetInstance(void);
  void UseContext(ReziContext *_context);
  void Awake(void);
  void Start(void);
  void Loop(void);
  void Update(void);
  void Render(void);
  void RenderGUI(void);
  void RenderNodeProps(void);
  void RenderDebugInfo(void);
  bool IsNodeHovered(size_t index, float radius);
  int GetHoveredNode(float radius);
  ~Editor(void);

private:
  static Editor *instance;
  Editor(int width, int height, bool fullscreen, std::string title);
  ReziContext *context;
  Font font;
  float nodeRadius, detailLineThick, connLineThick, forceLineThick, momentLineThick;
  std::string err_msg, status_msg;
  float guiHeight;
  Camera2D camera;
  TextBox<float> coordsBoxX, coordsBoxY, forceBoxX, forceBoxY, momentBox;
  EditorMode editorMode;
  int selectionNodesIndex[2];
  Button addNodeButton, nodeTypeButton, deleteNodeButton, addLineButton, deleteLineButton;
  int width, height;
  bool fullscreen;
  std::string title;
};
