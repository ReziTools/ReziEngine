#include "Utils.hpp"

void DrawTriangleLinesEx(Vector2 v1, Vector2 v2, Vector2 v3, float thick, Color color) {
  DrawLineEx(v1, v2, thick, color);
  DrawLineEx(v2, v3, thick, color);
  DrawLineEx(v3, v1, thick, color);
}

void DrawJoint(Vec2D position, float scale, float thick, Color color) {
  DrawLineEx({position.x(), position.y() + 20 / scale},
             {position.x(), position.y() - 20 / scale}, thick / scale, BLACK);
}

void DrawArticulation(Vec2D position, float scale, float thick, Color color) {
  DrawTriangleLinesEx(
      position,
      {position.x() + 5 / scale, position.y() - 10 / scale},
      {position.x() - 5 / scale, position.y() - 10 / scale},
      thick / scale, BLACK);
  DrawLineEx(
      {position.x() + 10 / scale, position.y() - 10 / scale},
      {position.x() - 10 / scale, position.y() - 10 / scale},
      thick / scale, BLACK);
}

void DrawBearing(Vec2D position, float scale, float thick, Color color) {
  DrawTriangleLinesEx(
      position,
      {position.x() + 5 / scale, position.y() - 10 / scale},
      {position.x() - 5 / scale, position.y() - 10 / scale},
      thick / scale, BLACK);
  DrawLineEx(
      {position.x() + 10 / scale, position.y() - 10 / scale},
      {position.x() - 10 / scale, position.y() - 10 / scale},
      thick / scale, BLACK);
  DrawLineEx(
      {position.x() + 10 / scale, position.y() - 15 / scale},
      {position.x() - 10 / scale, position.y() - 15 / scale},
      thick / scale, BLACK);
}

void DrawNode(Node node, float scale, float thick, Color color) {
  switch (node.type) {
  case NODE_FREE:
    break;
  case NODE_JOINT:
    DrawJoint(node.position, scale, thick, color);
    break;
  case NODE_ARTICULATION:
    DrawArticulation(node.position, scale, thick, color);
    break;
  case NODE_BEARING:
    DrawBearing(node.position, scale, thick, color);
    break;
  }
}

void DrawVector(Vec2D vec, Vec2D position, float scale, float thick, Color color) {
  if (vec.norm() < 0.001f)
    return;
  DrawLineEx(position, {position.x() + vec.x() * scale, position.y() + vec.y() * scale}, thick / scale, color);
}

void DrawMoment(float moment, Vec2D position, float scale, float thick, Color color) {
  if (abs(moment) < 0.001f)
    return;
  if (moment > 0.0f)
    DrawCircleSectorLines(position, moment / scale, 0.0f, 270.0f, 32, color);
  else
    DrawCircleSectorLines(position, -moment / scale, 270.0f, 0.0f, 32, color);
}
