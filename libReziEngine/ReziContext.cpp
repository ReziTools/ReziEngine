#include "ReziContext.hpp"
#include <fstream>
#include <iostream>
#include <toml.hpp>

std::string getNodeTypeName(NodeType type) {
  switch (type) {
  case NodeType::NODE_JOINT:
    return "joint";
  case NodeType::NODE_ARTICULATION:
    return "articulation";
  case NodeType::NODE_BEARING:
    return "bearing";
  case NodeType::NODE_FREE:
    return "free";
  }
}

size_t ReziContext::GetNodeCount(void) const { return Nodes.size(); }

void ReziContext::Resize(size_t size) {
  if (size < GetNodeCount())
    return;
  Nodes.resize(size);
  Connections.resize(size);
  for (std::vector<size_t> &row : Connections)
    row.resize(size);
}

void ReziContext::AddNode(Node node) {
  Nodes.push_back(node);
  Connections.resize(GetNodeCount());
  for (std::vector<size_t> &row : Connections)
    row.resize(GetNodeCount());
}

void ReziContext::DeleteNode(size_t index) {
  Nodes.erase(Nodes.begin() + index);
  Connections.erase(Connections.begin() + index);
  for (std::vector<size_t> &row : Connections)
    row.erase(row.begin() + index);
}

void ReziContext::SaveToml(const std::string path, std::string &err) const {
}

void ReziContext::LoadToml(const std::string path, std::string &err) {
}
