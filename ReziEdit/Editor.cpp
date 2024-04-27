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
    throw std::runtime_error("Context not provided, exiting.");
  SetConfigFlags(FLAG_MSAA_4X_HINT);
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
    if (editorMode == MODE_PAN && selectionNodesIndex[0] != -1)
      RenderNodeProps();
    EndDrawing();
  }
}

void Editor::Awake(void) {
  font = GetFontDefault();
  guiHeight = 72.0f;
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
  nodeTypeButton = {
      .position = {290.0f, 20.0f},
      .size = {90.0f, 16.0f},
      .label = NodeTypeNames.at(NODE_FREE),
      .normalColor = WHITE,
      .hoverColor = WHITE,
      .clickColor = WHITE};
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
  deleteNodeButton = {
      .position = {600.0f, 20.0f},
      .size = {64.0f, 16.0f},
      .label = "Delete",
      .normalColor = RED,
      .hoverColor = RED,
      .clickColor = RED};

  coordsBoxX.font = &font;
  coordsBoxX.position = {260.0f, 36.0f};
  coordsBoxX.size = {120.0f, 16.0f};
  coordsBoxX.fontSize = 16.0f;

  coordsBoxY.font = &font;
  coordsBoxY.position = {260.0f, 52.0f};
  coordsBoxY.size = {120.0f, 16.0f};
  coordsBoxY.fontSize = 16.0f;

  forceBoxX.font = &font;
  forceBoxX.position = {488.0f, 20.0f};
  forceBoxX.size = {100.0f, 16.0f};
  forceBoxX.fontSize = 16.0f;

  forceBoxY.font = &font;
  forceBoxY.position = {488.0f, 36.0f};
  forceBoxY.size = {100.0f, 16.0f};
  forceBoxY.fontSize = 16.0f;

  momentBox.font = &font;
  momentBox.position = {488.0f, 52.0f};
  momentBox.size = {100.0f, 16.0f};
  momentBox.fontSize = 16.0f;

  nodeRadius = 5.0f;
  connLineThick = 2.0f;
  detailLineThick = 1.25f;
  forceLineThick = 1.5f;

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

  if (addLineButton.IsClicked(MOUSE_BUTTON_LEFT)) {
    editorMode = MODE_ADDLINE;
  }
  if (deleteLineButton.IsClicked(MOUSE_BUTTON_LEFT)) {
    editorMode = MODE_DELLINE;
  }
  if (IsKeyPressed(KEY_E))
    context->EmitReziCode();
  if (IsKeyPressed(KEY_S))
    ReziSolver::FakeSolver(*context);
  if (IsKeyPressed(KEY_ESCAPE)) {
    editorMode = MODE_PAN;
    selectionNodesIndex[0] = -1;
  }
  if (GetMouseY() > guiHeight) {
    switch (editorMode) {
    case MODE_PAN: {
      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (GetHoveredNode(nodeRadius) != -1) {
          selectionNodesIndex[0] = GetHoveredNode(nodeRadius);
          coordsBoxX.target = &context->Nodes.at(selectionNodesIndex[0]).position.x();
          coordsBoxY.target = &context->Nodes.at(selectionNodesIndex[0]).position.y();
          forceBoxX.target = &context->Nodes.at(selectionNodesIndex[0]).cForce.x();
          forceBoxY.target = &context->Nodes.at(selectionNodesIndex[0]).cForce.y();
          momentBox.target = &context->Nodes.at(selectionNodesIndex[0]).cMoment;
          nodeTypeButton.label = NodeTypeNames.at(context->Nodes.at(selectionNodesIndex[0]).type);
        } else
          selectionNodesIndex[0] = -1;
      } else if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
        if (GetHoveredNode(nodeRadius * 2.0f) != -1)
          context->Nodes.at(GetHoveredNode(nodeRadius * 2.0f)).position = Vec2D(GetScreenToWorld2D(GetMousePosition(), camera));
      }
      break;
    }
    case MODE_ADDNODE: {
      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && GetHoveredNode(nodeRadius) == -1) {
        context->AddNode({.type = NODE_FREE,
                          .position = Vec2D(GetScreenToWorld2D(GetMousePosition(), camera)),
                          .cForce = {0.0f, 0.0f},
                          .cMoment = 0.0f});
      }
      break;
    }
    case MODE_ADDLINE: {
      if (selectionNodesIndex[0] == -1 && GetHoveredNode(nodeRadius) != -1 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        selectionNodesIndex[0] = GetHoveredNode(nodeRadius);
      else if (selectionNodesIndex[0] != -1 && selectionNodesIndex[1] == -1 && GetHoveredNode(nodeRadius) != -1 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        selectionNodesIndex[1] = GetHoveredNode(nodeRadius);
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
      if (selectionNodesIndex[0] == -1 && GetHoveredNode(nodeRadius) != -1 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        selectionNodesIndex[0] = GetHoveredNode(nodeRadius);
      else if (selectionNodesIndex[0] != -1 && selectionNodesIndex[1] == -1 && GetHoveredNode(nodeRadius) != -1 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        selectionNodesIndex[1] = GetHoveredNode(nodeRadius);
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
  if (selectionNodesIndex[0] != -1 && editorMode == MODE_PAN) {
    coordsBoxX.Update();
    coordsBoxY.Update();
    forceBoxX.Update();
    forceBoxY.Update();
    momentBox.Update();
    if (GetMouseY() < guiHeight) {
      if (nodeTypeButton.IsClicked(MOUSE_BUTTON_LEFT)) {
        switch (context->Nodes.at(selectionNodesIndex[0]).type) {
        case NODE_FREE:
          context->Nodes.at(selectionNodesIndex[0]).type = NODE_JOINT;
          break;
        case NODE_JOINT:
          context->Nodes.at(selectionNodesIndex[0]).type = NODE_ARTICULATION;
          break;
        case NODE_ARTICULATION:
          context->Nodes.at(selectionNodesIndex[0]).type = NODE_BEARING;
          break;
        case NODE_BEARING:
          context->Nodes.at(selectionNodesIndex[0]).type = NODE_FREE;
          break;
        }
        nodeTypeButton.label = NodeTypeNames.at(context->Nodes.at(selectionNodesIndex[0]).type);
      }
    }
    if (deleteNodeButton.IsClicked(MOUSE_BUTTON_LEFT)) {
      context->DeleteNode(selectionNodesIndex[0]);
      selectionNodesIndex[0] = -1;
    }
  }
}

void Editor::RenderDebugInfo(void) {
  DrawTextEx(font,
             TextFormat("FPS: %d\nNodeCount: %d\nHoveredNode: %d\nEditorMode: %d\nCamera: x:%.2f y:%.2f zoom:%.2f\nMouse: x:%.2f y:%.2f", GetFPS(), context->GetNodeCount(), GetHoveredNode(nodeRadius), editorMode, camera.target.x, camera.target.y, camera.zoom, GetScreenToWorld2D(GetMousePosition(), camera).x, GetScreenToWorld2D(GetMousePosition(), camera).y),
             {4.0f, guiHeight + 5.0f}, 16.0f, 2.0f, LIME);
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
        DrawLineEx(context->Nodes.at(i).position, context->Nodes.at(j).position, connLineThick / camera.zoom, BLACK);
  for (size_t i = 0; i < context->GetNodeCount(); i++) {
    Node node = context->Nodes.at(i);
    DrawNode(node, camera.zoom / 1.5f, detailLineThick, BLACK);
    DrawVector(node.cForce, node.position, 0.5f, forceLineThick / camera.zoom, BLUE);
    DrawMoment(node.cMoment, node.position, 2.0f, momentLineThick / camera.zoom, BLUE);
    DrawVector(node.rForce, node.position, 0.5f, forceLineThick / camera.zoom, PURPLE);
    DrawMoment(node.rMoment, node.position, 2.0f, momentLineThick / camera.zoom, PURPLE);
    DrawCircleV(node.position, nodeRadius / camera.zoom, IsNodeHovered(i, nodeRadius) ? GRAY : BLACK);
  }
  if (selectionNodesIndex[0] != -1 && (editorMode == MODE_ADDLINE || editorMode == MODE_DELLINE)) {
    DrawCircleV(context->Nodes.at(selectionNodesIndex[0]).position, nodeRadius / camera.zoom, BLUE);
    DrawLineEx(context->Nodes.at(selectionNodesIndex[0]).position, GetScreenToWorld2D(GetMousePosition(), camera), connLineThick / camera.zoom, BLUE);
  } else if (selectionNodesIndex[0] != -1)
    DrawCircleV(context->Nodes.at(selectionNodesIndex[0]).position, nodeRadius / camera.zoom, GREEN);
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
  DrawTextEx(font, "Nodes", {nodeRadius, nodeRadius}, 16.0f, 2.0f, BLACK);
  DrawTextEx(font, "Connections", {72.0f, nodeRadius}, 16.0f, 2.0f, BLACK);
  addNodeButton.Render();
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
  forceBoxX.Render();
  forceBoxY.Render();
  momentBox.Render();
  nodeTypeButton.Render();
  deleteNodeButton.Render();
}
