#pragma once
#include "Vec2D.hpp"
#include <map>
#include <raylib.h>

enum PointType { NONE = 0, JOIN, ARTI, BEAR };

const std::map<PointType, std::string> PointTypeNames = {
    {PointType::NONE, "Nod liber"},
    {PointType::JOIN, "Incastrare"},
    {PointType::ARTI, "Articulatie"},
    {PointType::BEAR, "Reazem"}};

struct Point {
  Vec2D position;
  Color color;
  PointType type;
  float radius;
  Vec2D conForce;
  float conMoment;
  float mouseDistance = -1.0f;
  Point(Vector2 position, PointType type, float radius, Color color)
      : position(position), type(type), radius(radius), color(color) {
    conForce = {0.0f, 0.0f};
    conMoment = 0.0f;
  };
  void Render(Camera2D camera) {
    switch (type) {
    case JOIN:
      RenderJoin(camera);
      break;
    case ARTI:
      RenderArti(camera);
      break;
    case BEAR:
      RenderBear(camera);
      break;
    default:
      break;
    }
    DrawCircleV(position, radius / camera.zoom,
                (IsHovered(camera)) ? LIGHTGRAY : color);
  }
  void RenderJoin(Camera2D camera) {
    DrawLineV({position.x(), position.y() + 20 / camera.zoom},
              {position.x(), position.y() - 20 / camera.zoom}, BLACK);
  }
  void RenderArti(Camera2D camera) {
    DrawTriangleLines(
        position,
        {position.x() + 5 / camera.zoom, position.y() - 10 / camera.zoom},
        {position.x() - 5 / camera.zoom, position.y() - 10 / camera.zoom},
        BLACK);
    DrawLineV(
        {position.x() + 10 / camera.zoom, position.y() - 10 / camera.zoom},
        {position.x() - 10 / camera.zoom, position.y() - 10 / camera.zoom},
        BLACK);
  }
  void RenderBear(Camera2D camera) {
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
  }
  void RenderTooltip(Camera2D camera) {
    DrawText(TextFormat("{%.2f, %.2f}", position.x(), position.y()),
             GetWorldToScreen2D(position, camera).x,
             GetWorldToScreen2D(position, camera).y + radius + 15,
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
