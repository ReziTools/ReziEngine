#include "TextBox.hpp"

template <typename T>
void TextBox<T>::Render(void) const {
  DrawRectangleV(position, size, WHITE);
  DrawTextEx(*font, data.c_str(), position, fontSize, 2.0f, (active) ? RED : BLACK);
}

template <typename T>
void TextBox<T>::Update(void) {
  if (!target)
    return;
  if (IsClicked(MOUSE_BUTTON_LEFT)) {
    active = true;
    data.clear();
  } else if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && !IsClicked(MOUSE_BUTTON_LEFT))
    active = false;
  if (!active) {
    UpdateData();
    return;
  }
  char c = GetCharPressed();
  if (IsKeyPressed(KEY_BACKSPACE) && data.size())
    data.pop_back();
  else if (c)
    data.push_back(c);
  if (IsKeyPressed(KEY_ENTER)) {
    UpdateTarget();
    active = false;
  }
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
  return IsHovered() && IsMouseButtonDown(button);
}

template <>
void TextBox<std::string>::UpdateTarget(void) {
  *target = data;
}

template <>
void TextBox<float>::UpdateTarget(void) {
  try {
    *target = std::stof(data);
  } catch (const std::invalid_argument &e) {
    return;
  }
}

template <>
void TextBox<double>::UpdateTarget(void) {
  try {
    *target = std::stod(data);
  } catch (const std::invalid_argument &e) {
    return;
  }
}
