#pragma once
#include "Vec2D.hpp"
#include <raylib.h>
#include <string>

struct Button {
  Button(Vec2D position, Vec2D size, std::string label, Color color);
  void Render(void);
  bool IsHovered(void);
  bool IsClicked(int key);
  Vec2D position, size;
  std::string label;
  Color color;
};
