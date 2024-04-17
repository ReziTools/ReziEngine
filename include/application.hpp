#pragma once
#include "button.hpp"
#include "graph.hpp"
#include "point.hpp"
#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include <string>
#include <vector>

class Application {
public:
  Application(int width, int height, bool fullscreen, std::string title);
  ~Application();
  void Start(void);

private:
  Camera2D camera;
  unsigned mode;
  PointType nodeType;
  Button* moveButton;
  Button* nodeButton;
  Button* lineButton;
  Button* typeButton;
  std::size_t selectPoint1Index;
  std::size_t selectPoint2Index;
  Graph<unsigned> connectionGraph;
  void resetButtons(void);
  bool touchingButtons(void);
  std::vector<Point> points;
  std::size_t GetHoveredPointIndex(void);
  void RenderLines(void);
  void Setup(void);
  void Awake(void);
  void Update(void);
  void FixedUpdate(void);
  float DeltaTime;
  Vector2 MouseDelta;
  int width, height, target_fps = 0;
  bool running;
  bool fullscreen;
  std::string title;
};
