#include "Button.hpp"

void Button::Render(void) const {
  if (IsHovered() && IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    DrawRectangleV(position, size, clickColor);
  else if (IsHovered())
    DrawRectangleV(position, size, hoverColor);
  else
    DrawRectangleV(position, size, normalColor);
  DrawTextEx(*font, label.c_str(), {position.x() + textPadding.x(), position.y() + textPadding.y()}, fontSize, fontSpacing, textColor);
}

void Button::CalculateTextPadding(void) {
  textPadding = (size - (Vec2D)MeasureTextEx(*font, label.c_str(), fontSize, fontSpacing)) / 2.0f;
}

bool Button::IsHovered(void) const {
  if (!active)
    return false;
  return (GetMouseX() >= position.x() && GetMouseX() <= (position + size).x() &&
          GetMouseY() >= position.y() && GetMouseY() <= (position + size).y())
             ? true
             : false;
}

bool Button::IsClicked(int button) const {
  return IsHovered() && IsMouseButtonPressed(button);
}
