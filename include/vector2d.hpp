#pragma once
#include <cmath>
#include <raylib.h>

struct Vec2D {
  float x, y;
  Vec2D(const float x, const float y) : x(x), y(y) {}
  explicit Vec2D(const Vector2 rlVec) : x(rlVec.x), y(rlVec.y) {}
  operator Vector2() const {
    return {static_cast<float>(x), static_cast<float>(y)};
  }
  Vec2D& operator=(const Vec2D& other) {
    if (this != &other) {
      x = other.x;
      y = other.y;
    }
    return *this;
  }
  Vec2D operator+(const Vec2D& other) const {
    return Vec2D(x + other.x, y + other.y);
  }
  Vec2D operator-(const Vec2D& other) const {
    return Vec2D(x - other.x, y - other.y);
  }
  Vec2D operator*(const float scalar) const {
    return Vec2D(x * scalar, y * scalar);
  }
  Vec2D operator/(const float scalar) const {
    return Vec2D(x / scalar, y / scalar);
  }
  float dot(const Vec2D other) const { return x * other.x + y * other.y; }
  float mag(void) const { return std::sqrt(x * x + y * y); }
  Vec2D normal(void) const { return Vec2D(x / mag(), y / mag()); }
};
