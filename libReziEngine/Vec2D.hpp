#pragma once
#include <Eigen/Dense>
#include <raylib.h>

class Vec2D : public Eigen::Vector2f {
public:
  using Eigen::Vector2f::Vector2f;
  explicit Vec2D(const Vector2 &vec2) : Eigen::Vector2f(vec2.x, vec2.y) {}
  operator Vector2() const {
    return {static_cast<float>((*this)(0)), static_cast<float>((*this)(1))};
  }
};
