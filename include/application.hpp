#pragma once
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>

#include <string>

class Application {
public:
  Application() = default;
  Application(int width, int height, std::string title);
  void Start(void);
  virtual ~Application();
  float DeltaTime;
  Vector2 MouseDelta;

private:
  void Setup(void);
  virtual void Awake(void);
  virtual void Update(void);
  virtual void FixedUpdate(void);
  int width, height, target_fps = 0;
  bool running = false;
  std::string title;
};
