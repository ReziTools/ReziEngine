#pragma once
#include "Vec2D.hpp"
#include <raylib.h>
#include <string>

struct TextBox {
  void Update(void);
  void Render(void);
  void Edit(void);
  void Reset(void);
  Vec2D GetSize(void);
  bool done = false;
  std::string data = "";
  std::string tooltip = "";
  size_t maxLenght = 0;
  float fontSize = 32.0f;
  Vec2D position = {0.0f, 0.0f};
  Vec2D size = {0.0f, 0.0f};
  Vec2D padding = {2.0f, 5.0f};
};
