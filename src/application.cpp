#include "application.hpp"
#include <iostream>
#include <raylib.h>

Application::Application(int width, int height, bool fullscreen,
                         std::string title)
    : width(width), height(height), fullscreen(fullscreen), title(title) {
  running = false;
}

Application::~Application() {
  delete moveButton;
  delete lineButton;
  delete nodeButton;
  CloseWindow();
}

void Application::Start(void) {
  running = true;
  InitWindow(width, height, title.c_str());
  if (fullscreen)
    ToggleFullscreen();
  if (target_fps)
    SetTargetFPS(target_fps);
  this->Awake();
  while (running) {
    running = !WindowShouldClose();
    this->FixedUpdate();
    BeginDrawing();
    this->Update();
    EndDrawing();
    DeltaTime = GetFrameTime();
    MouseDelta = GetMouseDelta();
  }
}

void Application::Awake(void) {
  camera = {0};
  camera.zoom = 100.0f;
  camera.target = {0, 0};
  camera.offset = (Vector2){GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
  nodeButton =
      new Button(Vec2D(0, height - 50), Vec2D(100, 50), "Adauga nod", GRAY);
  moveButton =
      new Button(Vec2D(100, height - 50), Vec2D(100, 50), "Muta nod", GRAY);
  lineButton =
      new Button(Vec2D(200, height - 50), Vec2D(100, 50), "Linie", GRAY);
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
  nodeButton->color = GRAY;
  moveButton->color = GRAY;
  lineButton->color = GRAY;
}

bool Application::touchingButtons(void) {
  return nodeButton->IsHovered() || moveButton->IsHovered() ||
         lineButton->IsHovered();
}

std::size_t Application::GetHoveredPointIndex(void) {
  for (size_t i = 0; i < points.size(); i++) {
    if (points.at(i).IsHovered(camera))
      return i;
  }
  return -1;
}

void Application::Update(void) {
  ClearBackground(WHITE);
  BeginMode2D(camera);
  DrawSnapGrid();
  RenderLines();
  for (Point point : points) {
    point.Render(camera);
    point.RenderTooltip(camera);
  }
  EndMode2D();
  for (Point point : points) {
    point.RenderTooltip(camera);
  }
  moveButton->Render();
  nodeButton->Render();
  lineButton->Render();
  DrawText(TextFormat(
               "FPS:%d\nFRAMETIME:%f\nCAM_X:%f\nCAM_Y:%f\nCAM_ZOOM:%f\nMODE:%d",
               GetFPS(), DeltaTime, camera.target.x, camera.target.y,
               camera.zoom, mode),
           0, 0, 16, BLACK);
}

void Application::FixedUpdate(void) {
  if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
    Vector2 delta = MouseDelta;
    delta = Vector2Scale(delta, -1.0f / camera.zoom);
    camera.target = Vector2Add(camera.target, delta);
  }

  float wheel = GetMouseWheelMove();
  if (wheel != 0) {
    Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);
    camera.offset = GetMousePosition();
    camera.target = mouseWorldPos;
    const float zoomIncrement = 0.5f;
    camera.zoom += (wheel * zoomIncrement);
    if (camera.zoom < zoomIncrement)
      camera.zoom = zoomIncrement;
  }
  if (!touchingButtons()) {
    switch (mode) {
    case 1: {
      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
          GetHoveredPointIndex() == -1) {
        points.push_back(
            Point(GetScreenToWorld2D(GetMousePosition(), camera), 4, GRAY));
        connectionGraph.Resize(points.size());
      }
      break;
    }
    case 2: {
      selectPoint1Index = GetHoveredPointIndex();
      if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && selectPoint1Index != -1)
        points.at(selectPoint1Index).position =
            (Vec2D)GetScreenToWorld2D(GetMousePosition(), camera);
      if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && selectPoint1Index != -1) {
        std::cin >> points.at(selectPoint1Index).position.x >>
            points.at(selectPoint1Index).position.y;
      }
      break;
    }

    case 3: {
      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (selectPoint1Index == -1) {
          selectPoint1Index = GetHoveredPointIndex();
          if (selectPoint1Index != -1) {
            points.at(selectPoint1Index).color = GREEN;
          }
        } else if (selectPoint2Index == -1) {
          selectPoint2Index = GetHoveredPointIndex();
          if (selectPoint2Index != -1 &&
              selectPoint1Index != selectPoint2Index) {
            std::cout << "Linking nodes:" << selectPoint1Index << " and "
                      << selectPoint2Index << "\n";
            points.at(selectPoint1Index).color = GRAY;
            connectionGraph.at(selectPoint1Index, selectPoint2Index) = 1;
            connectionGraph.at(selectPoint2Index, selectPoint1Index) = 1;
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
}

void Application::RenderLines(void) {
  for (size_t i = 0; i < points.size(); i++)
    for (size_t j = i + 1; j < points.size(); j++)
      if (connectionGraph.at(i, j))
        DrawLineV(points.at(i).position, points.at(j).position, BLACK);
}
