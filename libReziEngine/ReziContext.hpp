#pragma once
#include "Vec2D.hpp"
#include <filesystem>
#include <map>
#include <string>
#include <vector>

#define LOW_CUTOFF 0.0001f

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

const std::map<NodeType, std::string> NodeTypeNamesRezi = {
    {NodeType::NODE_FREE, "free"},
    {NodeType::NODE_JOINT, "join"},
    {NodeType::NODE_ARTICULATION, "arti"},
    {NodeType::NODE_BEARING, "bear"}};

struct Node {
  NodeType type;
  Vec2D position;
  Vec2D cForce;
  float cMoment;
  Vec2D rForce;
  float rMoment;
};

struct ReziContext {
  std::vector<Node> Nodes;
  std::vector<std::vector<size_t>> Connections;
  size_t GetNodeCount(void) const;
  void AddNode(Node node);
  void DeleteNode(size_t index);
  void SaveReziCode(const std::filesystem::path path, std::string &err) const;
};
