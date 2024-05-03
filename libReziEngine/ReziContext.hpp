#pragma once
#include "Vec2D.hpp"
#include <string>
#include <vector>

#define LOW_CUTOFF 0.0001f

enum NodeType {
  NODE_INVALID = 0,
  NODE_FREE,
  NODE_JOINT,
  NODE_ARTICULATION,
  NODE_BEARING
};

std::string getNodeTypeName(NodeType type);

struct Node {
  NodeType type = NodeType::NODE_INVALID;
  Vec2D position = {0.0f, 0.0f};
  Vec2D cForce = {0.0f, 0.0f};
  float cMoment = 0.0f;
  Vec2D rForce = {0.0f, 0.0f};
  float rMoment = 0.0f;
};

struct ReziContext {
  std::vector<Node> Nodes;
  std::vector<std::vector<size_t>> Connections;
  size_t GetNodeCount(void) const;
  void Resize(size_t size);
  void AddNode(Node node);
  void DeleteNode(size_t index);
  void Connect(size_t index1, size_t index2);
  void Disconnect(size_t index1, size_t index2);
  void SortX(void);
  void Clear(void);
  void SaveToml(const std::string path, std::string &err) const;
  void LoadToml(const std::string path, std::string &err);
};
