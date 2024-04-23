#include "Button.hpp"

void Button::Render(void) {
  if (IsHovered() && IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    DrawRectangleV(position, size, clickColor);
  else if (IsHovered())
    DrawRectangleV(position, size, hoverColor);
  else
    DrawRectangleV(position, size, normalColor);
  DrawText(label.c_str(), position.x(), position.y(), 16, BLACK);
}

bool Button::IsHovered(void) {
  return (GetMouseX() >= position.x() && GetMouseX() <= (position + size).x() &&
          GetMouseY() >= position.y() && GetMouseY() <= (position + size).y())
             ? true
             : false;
}

bool Button::IsClicked(int button) {
  return IsHovered() && IsMouseButtonPressed(button);
}
