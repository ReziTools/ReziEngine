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
  MODE_PROPS,
  MODE_CONNECT,
  MODE_DISCONNECT
};

class Editor {
public:
  static Editor &GetInstance(int width, int height, int fullscreen, std::string title, ReziContext &context);
  static Editor &GetInstance(void);
  void UseContext(ReziContext &_context);
  void Awake(void);
  void Start(void);
  void Loop(void);
  void Update(void);
  void UpdateGuiComponents(void);
  void Render(void);
  void RenderGUI(void);
  void RenderTopBar(void);
  void RenderNodeProps(void);
  void RenderNodeGroupProps(void);
  void RenderDebugInfo(void);
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
  std::vector<bool> SelectedNodes;
  int ClickedNode;
  void UpdateSelectedNodes(void);
  Button saveButton, loadButton, lockYButton;
  int width, height;
  bool fullscreen;
  std::string title;
  ReziContext &context;
};
