#pragma once
#include "Vec2D.hpp"
#include <raylib.h>
#include <string>

struct Button {
  void Render(void);
  bool IsHovered(void);
  bool IsClicked(int button);
  Vec2D position, size;
  std::string label;
  Color normalColor, hoverColor, clickColor;
};
