#include "TextBox.hpp"

template <typename T>
void TextBox<T>::Render(void) const {
  DrawTextEx(*font, data.c_str(), position, fontSize, 2.0f, (active) ? RED : BLACK);
}

template <typename T>
void TextBox<T>::Update(void) {
  if (!target)
    return;
  if (IsClicked(MOUSE_BUTTON_LEFT))
    active = true;
  else if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && !IsClicked(MOUSE_BUTTON_LEFT))
    active = false;
  if (!active)
    UpdateData();
  char c = GetCharPressed();
  if (IsKeyPressed(KEY_BACKSPACE) && data.size())
    data.pop_back();
  else if (c)
    data.push_back(c);
}

template <typename T>
void TextBox<T>::UpdateData(void) {
  data = std::to_string(*target);
}

template <>
void TextBox<std::string>::UpdateData(void) {
  data = *target;
}

template <typename T>
bool TextBox<T>::IsHovered(void) const {
  return (GetMouseX() >= position.x() && GetMouseX() <= (position + size).x() &&
          GetMouseY() >= position.y() && GetMouseY() <= (position + size).y())
             ? true
             : false;
}

template <typename T>
bool TextBox<T>::IsClicked(int button) const {
  return IsHovered() && IsMouseButtonPressed(button);
}
