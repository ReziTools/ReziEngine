#pragma once
#include "Vec2D.hpp"
#include <raylib.h>
#include <string>

template <typename T>
class TextBox {
public:
  void Render(void) const;
  void Update(void);
  bool IsHovered(void) const;
  bool IsClicked(int button) const;
  bool active = false;
  T *target = nullptr;
  Font *font = nullptr;
  Vec2D position = {0.0f, 0.0f};
  Vec2D size = {0.0f, 0.0f};
  float fontSize = 32.0f;

private:
  void UpdateData(void);
  void UpdateTarget(void);
  std::string data;
};

