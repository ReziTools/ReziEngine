#include "Application.hpp"

Application::Application(int width, int height, bool fullscreen,
                         std::string title)
    : width(width), height(height), fullscreen(fullscreen), title(title) {
  running = false;
  nodeType = NONE;
  coordsMode = 0;
  coordsPos = {0.0f, 0.0f};
}

Application::~Application() {
  delete moveButton;
  delete lineButton;
  delete nodeButton;
  delete typeButton;
  CloseWindow();
}

void Application::Start(void) {
  running = true;
  SetConfigFlags(FLAG_MSAA_4X_HINT);
  InitWindow(width, height, title.c_str());
  if (fullscreen)
    ToggleFullscreen();
  if (target_fps)
    SetTargetFPS(target_fps);
  this->Awake();
  while (running) {
    running = !WindowShouldClose();
    this->Update();
    BeginDrawing();
    this->Render();
    EndDrawing();
    DeltaTime = GetFrameTime();
    MouseDelta = (Vec2D)GetMouseDelta();
  }
}

void Application::Awake(void) {
  camera = {0};
  camera.zoom = 100.0f;
  camera.target = {0, 0};
  camera.rotation = 180.0f;
  camera.offset = (Vector2){GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
  nodeButton = new Button(Vec2D(5, height - 55), Vec2D(150, 50),
                          "Adauga/sterge\nnod", GRAY);
  typeButton = new Button(Vec2D(160, height - 55), Vec2D(100, 50),
                          PointTypeNames.at(nodeType), GRAY);
  moveButton =
      new Button(Vec2D(265, height - 55), Vec2D(100, 50), "Muta nod", GRAY);
  lineButton =
      new Button(Vec2D(370, height - 55), Vec2D(100, 50), "Linie", GRAY);
  mode = 0;
  selectPoint1Index = -1;
  selectPoint2Index = -1;
}

void DrawSnapGrid(void) {
  rlPushMatrix();
  rlTranslatef(0, 250, 0);
  rlRotatef(90, 1, 0, 0);
  DrawGrid(1000, 1);
  rlPopMatrix();
}

void Application::resetButtons(void) {
  if (selectPoint1Index != -1)
    points.at(selectPoint1Index).color = GRAY;
  nodeButton->color = GRAY;
  moveButton->color = GRAY;
  lineButton->color = GRAY;
}

bool Application::touchingButtons(void) {
  return nodeButton->IsHovered() || moveButton->IsHovered() ||
         lineButton->IsHovered() || typeButton->IsHovered();
}

std::size_t Application::GetHoveredPointIndex(void) {
  for (size_t i = 0; i < points.size(); i++) {
    if (points.at(i).IsHovered(camera))
      return i;
  }
  return -1;
}

void Application::Render(void) {
  ClearBackground(WHITE);
  BeginMode2D(camera);
  DrawSnapGrid();
  RenderLines();
  for (Point point : points) {
    point.Render(camera);
  }
  EndMode2D();
  for (Point point : points) {
    if (point.IsHovered(camera) || mode == 2)
      point.RenderTooltip(camera);
  }
  nodeButton->Render();
  typeButton->Render();
  moveButton->Render();
  lineButton->Render();
  DrawText(TextFormat("FPS:%d\nFRAMETIME:%f\nCAM_X:%f\nCAM_Y:%f\nCAM_ZOOM:%"
                      "f\nMODE:%d\nCOORD MODE:%d\nSEL NODE ID:%d",
                      GetFPS(), DeltaTime, camera.target.x, camera.target.y,
                      camera.zoom, mode, coordsMode, selectPoint1Index),
           0, 0, 16, BLACK);
  if (coordsMode)
    coordsBox.Render();
}

void Application::Update(void) {
  if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
    Vector2 delta = MouseDelta;
    delta = Vector2Scale(delta, -1.0f / camera.zoom);
    camera.target = Vector2Subtract(camera.target, delta);
  }

  float wheel = GetMouseWheelMove();
  if (wheel != 0) {
    Vec2D mouseWorldPos = (Vec2D)GetScreenToWorld2D(GetMousePosition(), camera);
    camera.offset = GetMousePosition();
    camera.target = mouseWorldPos;
    const float zoomIncrement = 1.0f;
    camera.zoom += (wheel * zoomIncrement);
    if (camera.zoom < zoomIncrement)
      camera.zoom = zoomIncrement;
  }
  if (!touchingButtons()) {
    switch (mode) {
    case 1: {
      selectPoint1Index = GetHoveredPointIndex();
      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && selectPoint1Index == -1) {
        points.push_back(Point(GetScreenToWorld2D(GetMousePosition(), camera),
                               nodeType, 4, BLACK));
        connectionGraph.Resize(points.size());
        selectPoint1Index = -1;
      }
      if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && selectPoint1Index != -1) {
        points.erase(points.begin() + selectPoint1Index);
        connectionGraph.Remove(selectPoint1Index);
        selectPoint1Index = -1;
      }
      break;
    }
    case 2: {
      if (selectPoint1Index == -1 && coordsMode == 0 &&
          (IsMouseButtonDown(MOUSE_BUTTON_LEFT) ||
           IsMouseButtonDown(MOUSE_BUTTON_RIGHT)))
        selectPoint1Index = GetHoveredPointIndex();
      if (!IsMouseButtonDown(MOUSE_BUTTON_LEFT) &&
          !IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && coordsMode == 0)
        selectPoint1Index = -1;
      if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && selectPoint1Index != -1)
        points.at(selectPoint1Index).position =
            (Vec2D)GetScreenToWorld2D(GetMousePosition(), camera);
      if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && selectPoint1Index != -1) {
        if (coordsMode == 0) {
          coordsMode = 1;
          coordsBox = (TextBox){
              .position = Vec2D(10.0f, 10.0f) +
                          (Vec2D)GetWorldToScreen2D(
                              points.at(selectPoint1Index).position, camera),
              .size = {128.0f, 0.0f}};
        }
      }
      break;
    }

    case 3: {
      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (selectPoint1Index == -1) {
          selectPoint1Index = GetHoveredPointIndex();
          if (selectPoint1Index != -1) {
            points.at(selectPoint1Index).color = BLUE;
          }
        } else if (selectPoint2Index == -1) {
          selectPoint2Index = GetHoveredPointIndex();
          if (selectPoint2Index != -1 &&
              selectPoint1Index != selectPoint2Index) {
            points.at(selectPoint1Index).color = BLACK;
            if (connectionGraph.at(selectPoint1Index, selectPoint2Index)) {
              connectionGraph.at(selectPoint1Index, selectPoint2Index) = 0;
              connectionGraph.at(selectPoint2Index, selectPoint1Index) = 0;
            } else {
              connectionGraph.at(selectPoint1Index, selectPoint2Index) = 1;
              connectionGraph.at(selectPoint2Index, selectPoint1Index) = 1;
            }
            selectPoint1Index = -1;
            selectPoint2Index = -1;
          }
        }
      }
      break;
    }
    }
  }
  for (Point& point : points) {
    point.Update(camera);
  }

  if (nodeButton->IsClicked(MOUSE_BUTTON_LEFT)) {
    mode = 1;
    resetButtons();
    nodeButton->color = GREEN;
    selectPoint1Index = -1;
    selectPoint2Index = -1;
  }
  if (moveButton->IsClicked(MOUSE_BUTTON_LEFT)) {
    mode = 2;
    resetButtons();
    moveButton->color = GREEN;
    selectPoint1Index = -1;
    selectPoint2Index = -1;
  }
  if (lineButton->IsClicked(MOUSE_BUTTON_LEFT)) {
    mode = 3;
    resetButtons();
    lineButton->color = GREEN;
    selectPoint1Index = -1;
    selectPoint2Index = -1;
  }
  if (typeButton->IsClicked(MOUSE_BUTTON_LEFT)) {
    switch (nodeType) {
    case NONE:
      nodeType = JOIN;
      break;
    case JOIN:
      nodeType = ARTI;
      break;
    case ARTI:
      nodeType = BEAR;
      break;
    case BEAR:
      nodeType = NONE;
      break;
    }
    typeButton->label = PointTypeNames.at(nodeType);
  }
  switch (coordsMode) {
  case 1: {
    coordsBox.Edit();
    if (coordsBox.done) {
      coordsMode = 2;
      try {
        coordsPos.x() = std::stof(coordsBox.data);
      } catch (const std::invalid_argument& e) {
        coordsMode = 0;
        coordsBox.Reset();
        selectPoint1Index = -1;
      }
      coordsBox.Reset();
    }
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      coordsMode = 0;
      selectPoint1Index = -1;
    }
    break;
  }
  case 2: {
    coordsBox.Edit();
    if (coordsBox.done) {
      coordsMode = 0;
      try {
        coordsPos.y() = std::stof(coordsBox.data);
      } catch (const std::invalid_argument& e) {
        coordsMode = 0;
        coordsBox.Reset();
        selectPoint1Index = -1;
      }
      coordsBox.Reset();
      points.at(selectPoint1Index).position = coordsPos;
      selectPoint1Index = -1;
    }
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      coordsMode = 0;
      selectPoint1Index = -1;
    }
    break;
  }
  }
}

void Application::RenderLines(void) {
  for (size_t i = 0; i < points.size(); i++)
    for (size_t j = i + 1; j < points.size(); j++)
      if (connectionGraph.at(i, j))
        DrawLineV(points.at(i).position, points.at(j).position, BLACK);
}
