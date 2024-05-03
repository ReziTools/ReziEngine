#pragma once
#include "Vec2D.hpp"
#include <raylib.h>
#include <string>

struct Button {
  void Render(void) const;
  bool IsHovered(void) const;
  bool IsClicked(int button) const;
  void CalculateTextPadding(void);
  Vec2D position;
  Vec2D textPadding = {0.0f, 0.0f};
  Vec2D size;
  float fontSize = 16.0f;
  float fontSpacing = 2.0f;
  std::string label;
  Font *font;
  Color textColor = BLACK;
  Color normalColor = GRAY;
  Color hoverColor = LIGHTGRAY;
  Color clickColor = WHITE;
};
