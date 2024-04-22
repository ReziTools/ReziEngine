#include "Editor.hpp"

Editor::Editor(int width, int height, bool fullscreen, std::string title) : width(width), height(height), fullscreen(fullscreen), title(title) {
  context = nullptr;
}

Editor::~Editor(void) {
  UnloadFont(font);
  CloseWindow();
}

void Editor::UseContext(ReziContext *_context) {
  context = _context;
}

void Editor::Start(void) {
  if (context == nullptr)
    throw "Context not provided, exiting.";
  InitWindow(width, height, title.c_str());
  SetWindowState(FLAG_WINDOW_RESIZABLE);
  SetExitKey(0);
  Awake();
  if (fullscreen)
    ToggleFullscreen();
  while (!WindowShouldClose()) {
    Update();
    BeginDrawing();
    ClearBackground(BLACK);
    Render();
    RenderGUI();
    if (selectionNodesIndex[0] != -1)
      RenderNodeProps();
    EndDrawing();
  }
}

void Editor::Awake(void) {
  font = GetFontDefault();
  guiHeight = 100.0f;
  camera = {
      .offset = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f},
      .target = {0.0f, 0.0f},
      .rotation = 180.0f,
      .zoom = 100.0f};

  addNodeButton = {
      .position = {4.0f, 20.0f},
      .size = {32.0f, 32.0f},
      .label = "+",
      .normalColor = WHITE,
      .hoverColor = LIGHTGRAY,
      .clickColor = GREEN};
  addLineButton = {
      .position = {72.0f, 20.0f},
      .size = {32.0f, 32.0f},
      .label = "+",
      .normalColor = WHITE,
      .hoverColor = LIGHTGRAY,
      .clickColor = GREEN};
  deleteLineButton = {
      .position = {108.0f, 20.0f},
      .size = {32.0f, 32.0f},
      .label = "-",
      .normalColor = WHITE,
      .hoverColor = LIGHTGRAY,
      .clickColor = GREEN};

  coordsBoxX.font = &font;
  coordsBoxX.position = {260.0f, 36.0f};
  coordsBoxX.size = {100.0f, 16.0f};
  coordsBoxX.fontSize = 16.0f;

  coordsBoxY.font = &font;
  coordsBoxY.position = {260.0f, 52.0f};
  coordsBoxY.size = {100.0f, 16.0f};
  coordsBoxY.fontSize = 16.0f;
  nodeType = NODE_FREE;
  editorMode = MODE_PAN;
  selectionNodesIndex[0] = -1;
  selectionNodesIndex[1] = -1;
}

void Editor::Update(void) {
  if (GetMouseY() > 40) {
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
  }
  if (addNodeButton.IsClicked(MOUSE_BUTTON_LEFT)) {
    editorMode = MODE_ADDNODE;
  }
  if (nodeTypeButton.IsClicked(MOUSE_BUTTON_LEFT)) {
    switch (nodeType) {
    case NODE_FREE:
      nodeType = NODE_JOINT;
      break;
    case NODE_JOINT:
      nodeType = NODE_ARTICULATION;
      break;
    case NODE_ARTICULATION:
      nodeType = NODE_BEARING;
      break;
    case NODE_BEARING:
      nodeType = NODE_JOINT;
      break;
    }
    nodeTypeButton.label = NodeTypeNames.at(nodeType);
  }

  if (addLineButton.IsClicked(MOUSE_BUTTON_LEFT)) {
    editorMode = MODE_ADDLINE;
  }
  if (deleteLineButton.IsClicked(MOUSE_BUTTON_LEFT)) {
    editorMode = MODE_DELLINE;
  }
  if (IsKeyPressed(KEY_E))
    context->EmitReziCode();
  if (IsKeyPressed(KEY_ESCAPE)) {
    editorMode = MODE_PAN;
  }
  if (GetMouseY() > guiHeight) {
    switch (editorMode) {
    case MODE_PAN: {
      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (GetHoveredNode(6.0f) != -1) {
          selectionNodesIndex[0] = GetHoveredNode(6.0f);
          coordsBoxX.target = &context->Nodes.at(selectionNodesIndex[0]).position.x();
          coordsBoxY.target = &context->Nodes.at(selectionNodesIndex[0]).position.y();
        } else
          selectionNodesIndex[0] = -1;
      } else if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
        if (GetHoveredNode(8.0f) != -1)
          context->Nodes.at(GetHoveredNode(8.0f)).position = Vec2D(GetScreenToWorld2D(GetMousePosition(), camera));
      }
      if (selectionNodesIndex[0] != -1) {
        coordsBoxX.Update();
        coordsBoxY.Update();
      }
      break;
    }
    case MODE_ADDNODE: {
      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && GetHoveredNode(6.0f) == -1) {
        context->AddNode({.type = nodeType,
                          .position = Vec2D(GetScreenToWorld2D(GetMousePosition(), camera)),
                          .cForce = {0.0f, 0.0f},
                          .cMoment = 0.0f});
      }
      break;
    }
    case MODE_ADDLINE: {
      if (selectionNodesIndex[0] == -1 && GetHoveredNode(6.0f) != -1 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        selectionNodesIndex[0] = GetHoveredNode(6.0f);
      else if (selectionNodesIndex[0] != -1 && selectionNodesIndex[1] == -1 && GetHoveredNode(6.0f) != -1 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        selectionNodesIndex[1] = GetHoveredNode(6.0f);
      else if (selectionNodesIndex[0] == selectionNodesIndex[1]) {
        selectionNodesIndex[0] = -1;
        selectionNodesIndex[1] = -1;
      } else if (selectionNodesIndex[0] != -1 && selectionNodesIndex[1] != -1) {
        context->Connections.at(selectionNodesIndex[0]).at(selectionNodesIndex[1]) = 1;
        context->Connections.at(selectionNodesIndex[1]).at(selectionNodesIndex[0]) = 1;
        selectionNodesIndex[0] = -1;
        selectionNodesIndex[1] = -1;
      }
      break;
    }
    case MODE_DELLINE: {
      if (selectionNodesIndex[0] == -1 && GetHoveredNode(6.0f) != -1 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        selectionNodesIndex[0] = GetHoveredNode(6.0f);
      else if (selectionNodesIndex[0] != -1 && selectionNodesIndex[1] == -1 && GetHoveredNode(6.0f) != -1 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        selectionNodesIndex[1] = GetHoveredNode(6.0f);
      else if (selectionNodesIndex[0] == selectionNodesIndex[1]) {
        selectionNodesIndex[0] = -1;
        selectionNodesIndex[1] = -1;
      } else if (selectionNodesIndex[0] != -1 && selectionNodesIndex[1] != -1) {
        context->Connections.at(selectionNodesIndex[0]).at(selectionNodesIndex[1]) = 0;
        context->Connections.at(selectionNodesIndex[1]).at(selectionNodesIndex[0]) = 0;
        selectionNodesIndex[0] = -1;
        selectionNodesIndex[1] = -1;
      }
      break;
    }
    }
  }
}

void Editor::RenderDebugInfo(void) {
  DrawText(
      TextFormat("FPS: %d\nNodeCount: %d\nHoveredNode: %d\nEditorMode: %d\nCamera: x:%.2f y:%.2f zoom:%.2f\nMouse: x:%.2f y:%.2f", GetFPS(), context->GetNodeCount(), GetHoveredNode(6.0f), editorMode, camera.target.x, camera.target.y, camera.zoom, GetScreenToWorld2D(GetMousePosition(), camera).x, GetScreenToWorld2D(GetMousePosition(), camera).y),
      5, guiHeight + 5.0f, 16, LIME);
}

void RenderGrid(float interval) {
  rlPushMatrix();
  rlTranslatef(0.0f, 100.0f, 0.0f);
  rlRotatef(90.0f, 1.0f, 0.0f, 0.0f);
  DrawGrid(1000 / interval, interval);
  rlPopMatrix();
}

void Editor::Render(void) {
  ClearBackground(WHITE);
  BeginMode2D(camera);
  RenderGrid(1.0f);
  for (size_t i = 0; i < context->GetNodeCount(); i++)
    for (size_t j = i + 1; j < context->GetNodeCount(); j++)
      if (context->Connections.at(i).at(j))
        DrawLineV(context->Nodes.at(i).position, context->Nodes.at(j).position, BLACK);
  for (size_t i = 0; i < context->GetNodeCount(); i++) {
    Vec2D position = context->Nodes.at(i).position;
    switch (context->Nodes.at(i).type) {
    case NODE_FREE:
      break;
    case NODE_JOINT:
      DrawLineV({position.x(), position.y() + 20 / camera.zoom},
                {position.x(), position.y() - 20 / camera.zoom}, BLACK);
      break;
    case NODE_ARTICULATION:
      DrawTriangleLines(
          position,
          {position.x() + 5 / camera.zoom, position.y() - 10 / camera.zoom},
          {position.x() - 5 / camera.zoom, position.y() - 10 / camera.zoom},
          BLACK);
      DrawLineV(
          {position.x() + 10 / camera.zoom, position.y() - 10 / camera.zoom},
          {position.x() - 10 / camera.zoom, position.y() - 10 / camera.zoom},
          BLACK);
      break;
    case NODE_BEARING:
      DrawTriangleLines(
          position,
          {position.x() + 5 / camera.zoom, position.y() - 10 / camera.zoom},
          {position.x() - 5 / camera.zoom, position.y() - 10 / camera.zoom},
          BLACK);
      DrawLineV(
          {position.x() + 10 / camera.zoom, position.y() - 10 / camera.zoom},
          {position.x() - 10 / camera.zoom, position.y() - 10 / camera.zoom},
          BLACK);
      DrawLineV(
          {position.x() + 10 / camera.zoom, position.y() - 15 / camera.zoom},
          {position.x() - 10 / camera.zoom, position.y() - 15 / camera.zoom},
          BLACK);
      break;
    }
    DrawCircleV(context->Nodes.at(i).position, 4.0f / camera.zoom, IsNodeHovered(i, 6.0f) ? GRAY : BLACK);

    if (selectionNodesIndex[0] != -1 && (editorMode == MODE_ADDLINE || editorMode == MODE_DELLINE)) {
      DrawCircleV(context->Nodes.at(selectionNodesIndex[0]).position, 4.0 / camera.zoom, BLUE);
      DrawLineV(context->Nodes.at(selectionNodesIndex[0]).position, GetScreenToWorld2D(GetMousePosition(), camera), BLUE);
    }
    if (selectionNodesIndex[0] != -1)
      DrawCircleV(context->Nodes.at(selectionNodesIndex[0]).position, 4.0 / camera.zoom, GREEN);
  }
  EndMode2D();
}

bool Editor::IsNodeHovered(size_t index, float radius) {
  return ((Vec2D)GetWorldToScreen2D(context->Nodes.at(index).position, camera) - (Vec2D)GetMousePosition()).norm() < radius;
}

int Editor::GetHoveredNode(float radius) {
  for (size_t i = 0; i < context->Nodes.size(); i++)
    if (IsNodeHovered(i, radius))
      return i;
  return -1;
}

void Editor::RenderGUI(void) {
  DrawRectangle(0, 0, GetScreenWidth(), guiHeight, LIGHTGRAY);
  DrawTextEx(font, "Nodes", {4.0f, 4.0f}, 16.0f, 2.0f, BLACK);
  DrawTextEx(font, "Connections", {72.0f, 4.0f}, 16.0f, 2.0f, BLACK);
  addNodeButton.Render();
  deleteNodeButton.Render();
  addLineButton.Render();
  deleteLineButton.Render();
  RenderDebugInfo();
}

void Editor::RenderNodeProps(void) {
  DrawRectangle(236, 0, 6000, guiHeight, GRAY);
  DrawTextEx(font, TextFormat("Node (%d) properties:", selectionNodesIndex[0]), {240.0f, 4.0f}, 16.0f, 2.0f, BLACK);
  DrawTextEx(font, "Type:", {240.0f, 20.0f}, 16.0f, 2.0f, BLACK);
  DrawTextEx(font, "X:", {240.0f, 36.0f}, 16.0f, 2.0f, BLACK);
  DrawTextEx(font, "Y:", {240.0f, 52.0f}, 16.0f, 2.0f, BLACK);
  DrawTextEx(font, "CForce X:", {400.0f, 20.0f}, 16.0f, 2.0f, BLACK);
  DrawTextEx(font, "CForce Y:", {400.0f, 36.0f}, 16.0f, 2.0f, BLACK);
  DrawTextEx(font, " CMoment:", {400.0f, 52.0f}, 16.0f, 2.0f, BLACK);
  coordsBoxX.Render();
  coordsBoxY.Render();
}
