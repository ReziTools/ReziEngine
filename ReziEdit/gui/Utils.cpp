#include "Utils.hpp"

void DrawTriangleLinesEx(Vector2 v1, Vector2 v2, Vector2 v3, float thick, Color color) {
  DrawLineEx(v1, v2, thick, color);
  DrawLineEx(v2, v3, thick, color);
  DrawLineEx(v3, v1, thick, color);
}

void DrawJoint(Vec2D position, float scale, float thick, Color color) {
  DrawLineEx({position.x(), position.y() + 20 / scale},
             {position.x(), position.y() - 20 / scale}, thick / scale, color);
}

void DrawArticulation(Vec2D position, float scale, float thick, Color color) {
  DrawTriangleLinesEx(
      position,
      {position.x() + 5 / scale, position.y() - 10 / scale},
      {position.x() - 5 / scale, position.y() - 10 / scale},
      thick / scale, color);
  DrawLineEx(
      {position.x() + 10 / scale, position.y() - 10 / scale},
      {position.x() - 10 / scale, position.y() - 10 / scale},
      thick / scale, color);
}

void DrawBearing(Vec2D position, float scale, float thick, Color color) {
  DrawTriangleLinesEx(
      position,
      {position.x() + 5 / scale, position.y() - 10 / scale},
      {position.x() - 5 / scale, position.y() - 10 / scale},
      thick / scale, color);
  DrawLineEx(
      {position.x() + 10 / scale, position.y() - 10 / scale},
      {position.x() - 10 / scale, position.y() - 10 / scale},
      thick / scale, color);
  DrawLineEx(
      {position.x() + 10 / scale, position.y() - 15 / scale},
      {position.x() - 10 / scale, position.y() - 15 / scale},
      thick / scale, color);
}

void DrawNode(Node node, float scale, float thick, Color color) {
  switch (node.type) {
  case NODE_INVALID:
  case NODE_VIRTUAL:
  case NODE_FREE:
    break;
  case NODE_JOINT:
    DrawJoint(node.position, scale, thick, color);
    break;
  case NODE_ARTICULATION:
    DrawArticulation(node.position, scale, thick, color);
    break;
  case NODE_BEARING:
    DrawBearing(node.position, scale, thick, color);
    break;
  }
}

void DrawVector(Vec2D vec, Vec2D position, float scale, float thick, Color color) {
  if (vec.norm() == 0.0f)
    return;
  DrawLineEx(position, {position.x() + vec.x() * scale, position.y() + vec.y() * scale}, thick / scale, color);
}

void DrawMoment(float moment, Vec2D position, float scale, float thick, Color color) {
  if (moment == 0.0f)
    return;
  DrawRing(position, abs(moment) * scale - thick / 2.0f, moment * scale + thick / 2.0f, 0.0f, 270.0f, 32, color);
}

void RenderDiagramT(const ReziContext &context, float yOffset, float yScale) {
  float vForceS = 0.0f;
  for (const Node &node : context.Nodes) {
    Vec2D p = node.position + Vec2D(0.0f, yOffset);
    DrawLineEx(node.position, p, 0.01f, GRAY);
  }
  for (size_t i = 0; i < context.GetNodeCount() - 1; i++) {
    const Node &node1 = context.Nodes.at(i);
    const Node &node2 = context.Nodes.at(i + 1);
    vForceS += node1.cForce.y() + node1.rForce.y();
    DrawRectangleV(
        {node1.position.x(), yOffset},
        {node2.position.x() - node1.position.x(), vForceS * yScale},
        {0, 0, 0, 64});
  }
  Vec2D p1 = context.Nodes.at(0).position + Vec2D(0.25f, yOffset);
  Vec2D p2 = context.Nodes.at(context.GetNodeCount() - 1).position + Vec2D(-0.25f, yOffset);
  DrawLineEx(p1, p2, 0.05f, BLACK);
}

#include <iostream>

void RenderDiagramM(const ReziContext &context, float yOffset, float yScale) {
  float MomentSl = 0.0f;
  float MomentSr = 0.0f;
  float MomentSp = 0.0f;
  float vForceS = 0.0f;
  for (const Node &node : context.Nodes) {
    Vec2D p = node.position + Vec2D(0.0f, yOffset);
    DrawLineEx(node.position, p, 0.01f, GRAY);
  }
  for (size_t i = 0; i < context.GetNodeCount() - 1; i++) {
    const Node &node1 = context.Nodes.at(i);
    const Node &node2 = context.Nodes.at(i + 1);
    vForceS += node1.cForce.y() + node1.rForce.y();
    MomentSp = MomentSr;
    MomentSl += vForceS * abs((node2.position - node1.position).norm());
    MomentSr = MomentSl + node2.cMoment;
    DrawLineEx(
        {node1.position.x(), yOffset - MomentSp * yScale},
        {node2.position.x(), yOffset - MomentSl * yScale},
        0.05f,
        BLACK);
    std::cout << i + 1 << ": " << MomentSl << " " << MomentSr << "\n";
  }
  Vec2D p1 = context.Nodes.at(0).position + Vec2D(0.25f, yOffset);
  Vec2D p2 = context.Nodes.at(context.GetNodeCount() - 1).position + Vec2D(-0.25f, yOffset);
  DrawLineEx(p1, p2, 0.05f, BLACK);
}
