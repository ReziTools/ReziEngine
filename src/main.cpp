#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>

#include <vector>

#include "application.hpp"

struct Point {
  Vector2 position;
  Color color;
  float radius;
  float mouseDistance = -1.0f;
  Point(Vector2 position, float radius, Color color)
      : position(position), radius(radius), color(color){};
  void Render(Camera2D camera) {
    DrawCircleV(position, radius / camera.zoom,
                (IsHovered(camera)) ? RED : color);
  }
  void RenderTooltip(Camera2D camera) {
    DrawText(TextFormat("{%.2f, %.2f}", position.x, position.y),
             GetWorldToScreen2D(position, camera).x,
             GetWorldToScreen2D(position, camera).y + radius + 4,
             16 / camera.zoom, BLACK);
  }
  void Update(Camera2D camera) {
    mouseDistance = Vector2Length(Vector2Subtract(
        GetScreenToWorld2D(GetMousePosition(), camera), position));
  }
  bool IsHovered(Camera2D camera) {
    return (mouseDistance >= 0.0f && mouseDistance <= radius / camera.zoom)
               ? true
               : false;
  }
};

class MainApplication : public Application {
public:
  using Application::Application;
  Camera2D camera;
  std::vector<Point> points;

private:
  void Awake(void) override;
  void Update(void) override;
  void FixedUpdate(void) override;
  Point* GetHoveredPoint(void);
  void RenderLines(void);
};

void MainApplication::Awake(void) {
  SetTargetFPS(60);
  camera = {0};
  camera.zoom = 100.0f;
  camera.target = {0, 0};
  camera.offset = (Vector2){GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
}

void DrawSnapGrid(void) {
  rlPushMatrix();
  rlTranslatef(0, 250, 0);
  rlRotatef(90, 1, 0, 0);
  DrawGrid(1000, 1);
  rlPopMatrix();
}

Point* MainApplication::GetHoveredPoint(void) {
  for (size_t i = 0; i < points.size(); i++) {
    if (points.at(i).IsHovered(camera))
      return &points.at(i);
  }
  return NULL;
}

void MainApplication::Update(void) {
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
  DrawText(TextFormat(
               "FPS:%d\n\nFRAMETIME:%f\n\nCAM_X:%f\n\nCAM_Y:%f\n\nCAM_ZOOM:%f",
               GetFPS(), DeltaTime, camera.target.x, camera.target.y,
               camera.zoom),
           0, 0, 32, BLACK);
}

void MainApplication::FixedUpdate(void) {
  Point* hoveredPoint = GetHoveredPoint();
  if (!hoveredPoint) {
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
      Vector2 delta = MouseDelta;
      delta = Vector2Scale(delta, -1.0f / camera.zoom);
      camera.target = Vector2Add(camera.target, delta);
    } else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
      points.push_back(
          Point(GetScreenToWorld2D(GetMousePosition(), camera), 10, GRAY));
    }
  } else {
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
      hoveredPoint->position = GetScreenToWorld2D(GetMousePosition(), camera);
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
  for (Point& point : points) {
    point.Update(camera);
  }
}

void MainApplication::RenderLines(void) {
  for (size_t i = 0; i < points.size(); i++)
    for (size_t j = i + 1; j < points.size(); j++)
      DrawLineV(points.at(i).position, points.at(j).position, GREEN);
}

int main() {
  MainApplication mainApplication(800, 600, "REZISTENTA");
  mainApplication.Start();
  return 0;
}
