#include "vector2d.hpp"
#include <raylib.h>

struct Point {
  Vec2D position;
  Color color;
  float radius;
  float mouseDistance = -1.0f;
  Point(Vector2 position, float radius, Color color)
      : position(position), radius(radius), color(color){};
  void Render(Camera2D camera) {
    DrawCircleV(position, radius / camera.zoom,
                (IsHovered(camera)) ? LIGHTGRAY : color);
  }
  void RenderTooltip(Camera2D camera) {
    DrawText(TextFormat("{%.2f, %.2f}", position.x(), position.y()),
             GetWorldToScreen2D(position, camera).x,
             GetWorldToScreen2D(position, camera).y + radius + 4,
             16 / camera.zoom, BLACK);
  }
  void Update(Camera2D camera) {
    mouseDistance =
        ((Vec2D)GetScreenToWorld2D(GetMousePosition(), camera) - position)
            .norm();
  }
  bool IsHovered(Camera2D camera) {
    return (mouseDistance >= 0.0f && mouseDistance <= radius / camera.zoom)
               ? true
               : false;
  }
};
