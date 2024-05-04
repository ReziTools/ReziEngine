#pragma once
#include "ReziContext.hpp"
#include "Vec2D.hpp"
#include <raylib.h>

void DrawNode(Node node, float scale, float thick, Color color);
void DrawVector(Vec2D vec, Vec2D position, float scale, float thick, Color color);
void DrawMoment(float moment, Vec2D position, float scale, float thick, Color color);
void RenderDiagramT(const ReziContext &context, float yOffset, float yScale);
void RenderDiagramM(const ReziContext &context, float yOffset, float yScale);
