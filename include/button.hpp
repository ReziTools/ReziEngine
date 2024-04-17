#pragma once
#include "vector2d.hpp"
#include <raylib.h>
#include <string>

struct Button {
  Vec2D position, size;
  std::string label;
  Color color;
  Button(Vec2D position, Vec2D size, std::string label, Color color)
      : position(position), size(size), label(label), color(color) {}
  void Render(void) {
    DrawRectangleV(position, size, color);
    DrawText(label.c_str(), position.x(), position.y(), 16, BLACK);
  }
  bool IsHovered(void) {
    return (GetMouseX() >= position.x() &&
            GetMouseX() <= (position + size).x() &&
            GetMouseY() >= position.y() && GetMouseY() <= (position + size).y())
               ? true
               : false;
  }
  bool IsClicked(int key) { return IsHovered() && IsMouseButtonPressed(key); }
};
