#pragma once
#include "Vec2D.hpp"
#include <iostream>
#include <map>
#include <string>
#include <vector>

enum NodeType {
  NODE_FREE = 0,
  NODE_JOINT,
  NODE_ARTICULATION,
  NODE_BEARING
};

const std::map<NodeType, std::string> NodeTypeNames = {
    {NodeType::NODE_FREE, "Free"},
    {NodeType::NODE_JOINT, "Joint"},
    {NodeType::NODE_ARTICULATION, "Articulation"},
    {NodeType::NODE_BEARING, "Bearing"}};

struct Node {
  NodeType type;
  Vec2D position;
  Vec2D cForce;
  float cMoment;
  Vec2D rForce;
  Vec2D rMoment;
};

struct ReziContext {
  std::vector<Node> Nodes;
  std::vector<std::vector<size_t>> Connections;
  size_t GetNodeCount(void) const;
  void AddNode(Node node);
  void DeleteNode(size_t index);
  void EmitReziCode(void) const;
};
