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
  bool done;
  std::string data;
  size_t maxLenght;
  float fontSize;
  Vec2D position;
  Vec2D size;
  Vec2D padding;
};
