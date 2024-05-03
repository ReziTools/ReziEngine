#include "Editor.hpp"
#include "ReziSolver.hpp"
#include "gui/Utils.hpp"

Editor *Editor::instance = nullptr;

Editor::Editor(int width, int height, bool fullscreen, std::string title, ReziContext &context) : width(width), height(height), fullscreen(fullscreen), title(title), context(context) {
}

Editor &Editor::GetInstance(int width, int height, int fullscreen, std::string title, ReziContext &context) {
  if (!instance) {
    instance = new Editor(width, height, fullscreen, title, context);
  }
  return *instance;
}

Editor &Editor::GetInstance(void) {
  if (!instance)
    throw std::runtime_error("Instance not created!");
  return *instance;
}

Editor::~Editor(void) {
  UnloadFont(font);
  CloseWindow();
}

void RenderGrid(float interval) {
  rlPushMatrix();
  rlTranslatef(0.0f, 100.0f, 0.0f);
  rlRotatef(90.0f, 1.0f, 0.0f, 0.0f);
  DrawGrid(1000 / interval, interval);
  rlPopMatrix();
}

void Editor::Start(void) {
  SetConfigFlags(FLAG_MSAA_4X_HINT);
  InitWindow(width, height, title.c_str());
  SetWindowState(FLAG_WINDOW_RESIZABLE);
  SetExitKey(0);
  Awake();
  if (fullscreen)
    ToggleFullscreen();
#if defined(PLATFORM_WEB)
#else
  while (!WindowShouldClose()) {
    Loop();
  }
#endif
}

void Editor::Loop(void) {
  Update();
  UpdateGuiComponents();
  BeginDrawing();
  ClearBackground(BLACK);
  Render();
  RenderTopBar();
  RenderGUI();
  RenderNodeProps();
  EndDrawing();
}

void Editor::Awake(void) {
  font = GetFontDefault();
  camera = {
      .offset = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f},
      .target = {0.0f, 0.0f},
      .rotation = 180.0f,
      .zoom = 100.0f};

  saveButton.font = &font;
  saveButton.label = "Save";
  saveButton.size = {50.0f, 20.0f};
  saveButton.position = {0.0f, 0.0f};
  saveButton.CalculateTextPadding();

  loadButton.font = &font;
  loadButton.label = "Load";
  loadButton.size = {50.0f, 20.0f};
  loadButton.position = {saveButton.size.x(), 0.0f};
  loadButton.CalculateTextPadding();

  lockYButton.font = &font;
  lockYButton.label = "Lock Y";
  lockYButton.size = {60.0f, 20.0f};
  lockYButton.position = {loadButton.size.x() + loadButton.position.x(), 0.0f};
  lockYButton.CalculateTextPadding();

  nodeRadius = 6.0f;

  lockY = false;
  editorMode = MODE_FREE;
  ClickedNode = -1;
  errMsg.clear();
}

void Editor::Update(void) {
  auto getSelectedNodes = [=]() {
    std::vector<size_t> vec;
    for (size_t i = 0; i < SelectedNodes.size(); i++)
      if (SelectedNodes.at(i))
        vec.push_back(i);
    return vec;
  };

  guiHeight = 100.0f;

  if (IsKeyPressed(KEY_N))
    editorMode = EditorMode::MODE_ADD;
  if (IsKeyPressed(KEY_ESCAPE)) {
    SelectedNodes.assign(SelectedNodes.size(), false);
    editorMode = EditorMode::MODE_FREE;
  }

  if (GetMouseY() > guiHeight) {
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
      Vec2D delta = (Vec2D)GetMouseDelta();
      delta *= -1.0f / camera.zoom;
      camera.target = Vector2Subtract(camera.target, delta);
    }

    float wheel = GetMouseWheelMove();
    if (wheel != 0) {
      Vec2D mouseWorldPos = (Vec2D)GetScreenToWorld2D({(float)GetMouseX(), (float)GetMouseY()}, camera);
      camera.offset = GetMousePosition();
      camera.target = mouseWorldPos;
      const float zoomIncrement = 1.0f;
      camera.zoom += (wheel * zoomIncrement);
      if (camera.zoom < zoomIncrement)
        camera.zoom = zoomIncrement;
    }
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      int hoveredIndex = GetHoveredNode(nodeRadius);
      if (IsKeyDown(KEY_LEFT_SHIFT)) {
        if (hoveredIndex != -1) {
          if (SelectedNodes.at(hoveredIndex))
            SelectedNodes.at(hoveredIndex) = false;
          else
            SelectedNodes.at(hoveredIndex) = true;
        }
      } else if (hoveredIndex == -1) {
        SelectedNodes.assign(SelectedNodes.size(), false);
      }
    }
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
      ClickedNode = GetHoveredNode(nodeRadius * 1.5f);
    } else {
      ClickedNode = -1;
    }
  }
  std::vector<size_t> selNodes = getSelectedNodes();
  if (selNodes.size() == 2) {
    if (IsKeyPressed(KEY_C)) {
      context.Connect(selNodes.at(0), selNodes.at(1));
      SelectedNodes.assign(SelectedNodes.size(), false);
    }
    if (IsKeyPressed(KEY_D)) {
      context.Disconnect(selNodes.at(0), selNodes.at(1));
      SelectedNodes.assign(SelectedNodes.size(), false);
    }
  }
  if (selNodes.size()) {
    if (IsKeyPressed(KEY_DELETE)) {
      for (size_t i : selNodes)
        context.DeleteNode(i);
      SelectedNodes.assign(SelectedNodes.size(), false);
    }
  }
  if (IsKeyPressed(KEY_S)) {
    errMsg.clear();
    ReziSolver::SolveT(context, errMsg);
    if (errMsg.empty())
      statusMsg = "Solved system using T solver.";
  }
}

void Editor::UpdateGuiComponents(void) {
  if (lockYButton.IsClicked(MOUSE_BUTTON_LEFT)) {
    if (lockY) {
      lockY = true;
      lockYButton.textColor = LIME;
    } else {
      lockY = false;
      lockYButton.textColor = BLACK;
    }
  }
  if (loadButton.IsClicked(MOUSE_BUTTON_LEFT)) {
    errMsg.clear();
    context.LoadToml("out.toml", errMsg);
    SelectedNodes.clear();
    SelectedNodes.assign(context.GetNodeCount(), false);
    statusMsg = "Loaded out.toml.";
  }
}

void Editor::Render(void) {
  ClearBackground(WHITE);
  BeginMode2D(camera);
  RenderGrid(1.0f);
  for (size_t i = 0; i < context.GetNodeCount(); i++) {
    Node &node = context.Nodes.at(i);
    for (size_t j = i + 1; j < context.GetNodeCount(); j++)
      if (context.Connections.at(i).at(j))
        DrawLineEx(context.Nodes.at(i).position, context.Nodes.at(j).position, 3.0f / camera.zoom, BLACK);
    DrawNode(node, camera.zoom / 1.5f, nodeRadius / 2.0f, BLACK);
    DrawCircleV(node.position, nodeRadius / camera.zoom, IsNodeHovered(i, nodeRadius) ? GRAY : (node.type == NodeType::NODE_INVALID) ? RED
                                                                                                                                     : BLACK);
    if (ClickedNode == (int)i)
      DrawCircleV(node.position, nodeRadius / camera.zoom, GREEN);
  }
  for (size_t i = 0; i < context.GetNodeCount(); i++) {
    if (SelectedNodes.at(i))
      DrawCircleV(context.Nodes.at(i).position, nodeRadius / camera.zoom, IsNodeHovered(i, nodeRadius) ? YELLOW : ORANGE);
  }
  EndMode2D();
  RenderDebugInfo();
}

bool Editor::IsNodeHovered(size_t index, float radius) {
  if (lockY)
    return abs(GetWorldToScreen2D(context.Nodes.at(index).position, camera).x - GetMousePosition().x) < radius;
  return ((Vec2D)GetWorldToScreen2D(context.Nodes.at(index).position, camera) - (Vec2D)GetMousePosition()).norm() < radius;
}

int Editor::GetHoveredNode(float radius) {
  for (size_t i = 0; i < context.Nodes.size(); i++)
    if (IsNodeHovered(i, radius))
      return i;
  return -1;
}

void Editor::RenderGUI(void) {
  if (errMsg.length()) {
    DrawTextEx(font, errMsg.c_str(), {5.0f, GetScreenHeight() - 21.0f}, 16.0f, 2.0f, RED);
  } else if (statusMsg.length()) {
    DrawTextEx(font, statusMsg.c_str(), {5.0f, GetScreenHeight() - 21.0f}, 16.0f, 2.0f, GREEN);
  }
  DrawRectangleV({0.0f, 20.0f}, {(float)GetScreenWidth(), 80.0f}, LIGHTGRAY);
}

void Editor::RenderTopBar(void) {
  DrawRectangleV({0.0f, 0.0f}, {(float)GetScreenWidth(), 20.0f}, GRAY);
  DrawTextEx(font, "ReziEdit", {((float)GetScreenWidth() - MeasureTextEx(font, "ReziEdit", 16.0f, 2.0f).x - 2.0f), 2.0f}, 16.0f, 2.0f, BLACK);
  saveButton.Render();
  loadButton.Render();
  lockYButton.Render();
}

void Editor::RenderNodeProps(void) {
  DrawRectangleV({std::max((float)GetScreenWidth() / 3.0f, 200.0f), 20.0f}, {(float)GetScreenWidth(), 80.0f}, GRAY);
}

void Editor::RenderDebugInfo(void) {
  if (GetHoveredNode(nodeRadius) != -1)
    DrawTextEx(font,
               TextFormat("FPS: %d\nNodeCount: %d\nHovered Node: %d\nEditor Mode: %d\nCamera: x:%.2f y:%.2f zoom:%.2f\nMouse: x:%.2f y:%.2f", GetFPS(), context.GetNodeCount(), GetHoveredNode(nodeRadius) + 1, editorMode, camera.target.x, camera.target.y, camera.zoom, GetScreenToWorld2D(GetMousePosition(), camera).x, GetScreenToWorld2D(GetMousePosition(), camera).y),
               {4.0f, guiHeight + 5.0f}, 16.0f, 2.0f, LIME);
  else
    DrawTextEx(font,
               TextFormat("FPS: %d\nNodeCount: %d\nHovered Node: none\nEditor Mode: %d\nCamera: x:%.2f y:%.2f zoom:%.2f\nMouse: x:%.2f y:%.2f", GetFPS(), context.GetNodeCount(), editorMode, camera.target.x, camera.target.y, camera.zoom, GetScreenToWorld2D(GetMousePosition(), camera).x, GetScreenToWorld2D(GetMousePosition(), camera).y),
               {4.0f, guiHeight + 5.0f}, 16.0f, 2.0f, LIME);
}
