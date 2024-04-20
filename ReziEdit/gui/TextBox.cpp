#include "TextBox.hpp"

void TextBox::Update(void) {
  if (done)
    return;
  char c = GetCharPressed();
  if (IsKeyPressed(KEY_BACKSPACE) && data.size())
    data.pop_back();
  else if (c && (data.size() < maxLenght || !maxLenght))
    data.push_back(c);
}

void TextBox::Edit(void) {
  if (IsKeyPressed(KEY_ENTER) && data.length() > 0)
    done = true;
  else
    Update();
}

void TextBox::Render(void) {
  Vec2D pos = position;
  DrawRectangleV(pos, GetSize(), LIGHTGRAY);
  DrawText(data.c_str(), pos.x() + padding.x(), pos.y() + padding.y(), fontSize,
           BLACK);
}

void TextBox::Reset(void) {
  data.clear();
  done = false;
}

Vec2D TextBox::GetSize(void) {
  return {std::max(size.x(), (float)MeasureText(data.c_str(), fontSize)) +
              padding.x() * 2.0f,
          std::max(size.y(), fontSize) + padding.y() * 2.0f};
}
