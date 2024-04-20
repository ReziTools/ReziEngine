#include "Editor.hpp"

Editor::Editor(int width, int height, bool fullscreen, std::string title) : width(width), height(height), fullscreen(fullscreen), title(title) {
  context = nullptr;
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
    EndDrawing();
  }
}

void Editor::Awake(void) {
  guiHeight = 60.0f;
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
      .position = {40.0f, 20.0f},
      .size = {96.0f, 32.0f},
      .label = NodeTypeNames.at(NODE_FREE),
      .normalColor = WHITE,
      .hoverColor = LIGHTGRAY,
      .clickColor = GREEN};
  moveNodeButton = {
      .position = {140.0f, 20.0f},
      .size = {32.0f, 32.0f},
      .label = "M",
      .normalColor = WHITE,
      .hoverColor = LIGHTGRAY,
      .clickColor = GREEN};
  deleteNodeButton = {
      .position = {176.0f, 20.0f},
      .size = {32.0f, 32.0f},
      .label = "-",
      .normalColor = WHITE,
      .hoverColor = LIGHTGRAY,
      .clickColor = GREEN};
  addLineButton = {
      .position = {224.0f, 20.0f},
      .size = {32.0f, 32.0f},
      .label = "+",
      .normalColor = WHITE,
      .hoverColor = LIGHTGRAY,
      .clickColor = GREEN};
  deleteLineButton = {
      .position = {260.0f, 20.0f},
      .size = {32.0f, 32.0f},
      .label = "-",
      .normalColor = WHITE,
      .hoverColor = LIGHTGRAY,
      .clickColor = GREEN};

  coordsBoxX = {
      .done = 0,
      .data = "",
      .maxLenght = 8,
      .fontSize = 32.0f,
      .position = {4.0f, 64.0f},
      .size = {64.0f, 16.0f},
      .padding = {2.0f, 2.0f}};
  coordsBoxY = {
      .done = 0,
      .data = "",
      .maxLenght = 8,
      .fontSize = 32.0f,
      .position = {76.0f, 64.0f},
      .size = {64.0f, 16.0f},
      .padding = {2.0f, 2.0f}};
  coordsBoxActive = false;
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
  if (deleteNodeButton.IsClicked(MOUSE_BUTTON_LEFT)) {
    editorMode = MODE_DELNODE;
  }
  if (moveNodeButton.IsClicked(MOUSE_BUTTON_LEFT)) {
    editorMode = MODE_MOVNODE;
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
    coordsBoxX.Reset();
    coordsBoxY.Reset();
    coordsBoxActive = false;
  }
  if (GetMouseY() > guiHeight) {
    switch (editorMode) {
    case MODE_PAN:
      break;
    case MODE_ADDNODE: {
      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && GetHoveredNode(6.0f) == -1) {
        context->AddNode({.type = nodeType,
                          .position = Vec2D(GetScreenToWorld2D(GetMousePosition(), camera)),
                          .cForce = {0.0f, 0.0f},
                          .cMoment = 0.0f});
      }
      break;
    }
    case MODE_DELNODE: {
      if (GetHoveredNode(6.0f) != -1 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        context->DeleteNode(GetHoveredNode(6.0f));
      break;
    }
    case MODE_MOVNODE: {
      if (GetHoveredNode(12.0f) != -1 && IsMouseButtonDown(MOUSE_BUTTON_LEFT) && !coordsBoxActive)
        context->Nodes.at(GetHoveredNode(12.0f)).position = Vec2D(GetScreenToWorld2D(GetMousePosition(), camera));
      if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && GetHoveredNode(6.0f) != -1 && !coordsBoxActive) {
        coordsBoxActive = true;
        selectionNodesIndex[0] = GetHoveredNode(6.0f);
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
  if (coordsBoxActive) {
    Vec2D coords;
    coordsBoxY.position.x() = coordsBoxX.GetSize().x() + 8.0f;
    if (!coordsBoxX.done)
      coordsBoxX.Edit();
    else
      coordsBoxY.Edit();
    if (coordsBoxY.done) {
      try {
        coords.x() = std::stof(coordsBoxX.data);
        coords.y() = std::stof(coordsBoxY.data);
      } catch (const std::invalid_argument &e) {
        coordsBoxX.Reset();
        coordsBoxY.Reset();
        coordsBoxActive = false;
        selectionNodesIndex[0] = -1;
        return;
      }
      context->Nodes.at(selectionNodesIndex[0]).position = coords;
      coordsBoxX.Reset();
      coordsBoxY.Reset();
      coordsBoxActive = false;
      selectionNodesIndex[0] = -1;
      return;
    }
  }
  guiHeight = (coordsBoxActive) ? 104.0f : 60.0f;
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
  }
  EndMode2D();
  if (editorMode == MODE_MOVNODE) {
    for (Node node : context->Nodes) {
      std::string coordsText = TextFormat("x:%.2f y:%.2f", node.position.x(), node.position.y());
      DrawText(coordsText.c_str(), GetWorldToScreen2D(node.position, camera).x - MeasureText(coordsText.c_str(), 8) / 2.0f, GetWorldToScreen2D(node.position, camera).y + 4, 4, BLACK);
    }
  }
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
  DrawText("Nodes", 4, 4, 16, BLACK);
  DrawText("Connections", 224, 4, 16, BLACK);
  addNodeButton.Render();
  nodeTypeButton.Render();
  moveNodeButton.Render();
  deleteNodeButton.Render();
  addLineButton.Render();
  deleteLineButton.Render();
  if (coordsBoxActive) {
    DrawRectangle(0, 60, GetScreenWidth(), 44, GRAY);
    coordsBoxX.Render();
    coordsBoxY.Render();
  }
  RenderDebugInfo();
}

Editor::~Editor(void) {
  CloseWindow();
}
