#include "application.hpp"

#include <raylib.h>

#include <iostream>

Application::Application(int width, int height, std::string title)
    : width(width), height(height), title(title) {}

Application::~Application() { CloseWindow(); }

void Application::Start(void) {
  running = true;
  Setup();
  this->Awake();
  while (running) {
    running = !WindowShouldClose();
    this->FixedUpdate();
    BeginDrawing();
    this->Update();
    EndDrawing();
    DeltaTime = GetFrameTime();
    MouseDelta = GetMouseDelta();
  }
}

void Application::Awake(void) {
  std::cerr << "Override Application::Awake()!\n";
}

void Application::Setup(void) {
  InitWindow(width, height, title.c_str());
  if (target_fps)
    SetTargetFPS(target_fps);
}

void Application::Update(void) {
  ClearBackground(BLACK);
  DrawText("Override Application::Update()!", 0, 0, 32, RED);
}

void Application::FixedUpdate(void) {
  std::cerr << ("Override Application::FixedUpdate()!");
}
