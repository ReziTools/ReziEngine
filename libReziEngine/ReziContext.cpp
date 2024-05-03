#include "ReziContext.hpp"
#include <fstream>
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
  case NodeType::NODE_INVALID:
    return "invalid";
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

void ReziContext::SortX(void) {
  std::sort(Nodes.begin(), Nodes.end(), [](Node a, Node b) {
    return a.position.x() > b.position.x();
  });
}

void ReziContext::AddNode(Node node) {
  Nodes.push_back(node);
  Connections.resize(GetNodeCount());
  for (std::vector<size_t> &row : Connections)
    row.resize(GetNodeCount());
}

void ReziContext::Connect(size_t index1, size_t index2) {
  if (index1 == index2)
    return;
  Connections.at(index1).at(index2) = 1;
  Connections.at(index2).at(index1) = 1;
}

void ReziContext::Disconnect(size_t index1, size_t index2) {
  if (index1 == index2)
    return;
  Connections.at(index1).at(index2) = 0;
  Connections.at(index2).at(index1) = 0;
}

void ReziContext::DeleteNode(size_t index) {
  Nodes.erase(Nodes.begin() + index);
  Connections.erase(Connections.begin() + index);
  for (std::vector<size_t> &row : Connections)
    row.erase(row.begin() + index);
}

void ReziContext::Clear(void) {
  Nodes.clear();
  Connections.clear();
}

void ReziContext::SaveToml(const std::string path, std::string &err) const {
  std::ofstream fout(path);
  if (!fout.is_open()) {
    err = "Unable to write to file";
    return;
  }
  toml::value tomlData = toml::table();
  toml::array nodeArray;
  for (size_t i = 0; i < GetNodeCount(); i++) {
    const Node &node = Nodes[i];
    toml::table nodeData;
    std::vector<unsigned> conns;
    nodeData["index"] = i + 1;
    nodeData["type"] = getNodeTypeName(node.type);
    nodeData["x"] = node.position.x();
    nodeData["y"] = node.position.y();
    nodeData["cForceX"] = node.cForce.x();
    nodeData["cForceY"] = node.cForce.y();
    nodeData["cMoment"] = node.cMoment;
    for (size_t j = 0; j < GetNodeCount(); j++) {
      if (Connections.at(i).at(j))
        conns.push_back(j + 1);
    }
    nodeData["connections"] = conns;
    nodeArray.push_back(nodeData);
  }
  tomlData.as_table()["node"] = nodeArray;
  fout << tomlData;
  fout.close();
}

NodeType stringToNodeType(std::string str) {
  if (str == "free")
    return NodeType::NODE_FREE;
  if (str == "bearing")
    return NodeType::NODE_BEARING;
  if (str == "articulation")
    return NodeType::NODE_ARTICULATION;
  if (str == "joint")
    return NodeType::NODE_JOINT;
  return NodeType::NODE_INVALID;
}

void ReziContext::LoadToml(const std::string path, std::string &err) {
  err.clear();
  Clear();
  std::ifstream fin(path);
  if (!fin.is_open()) {
    err = "Unable to open file.";
    return;
  }
  toml::value toml_data = toml::parse(fin);
  toml::value nodeArray = toml::find(toml_data, "node");
  Resize(nodeArray.as_array().size());
  for (toml::value &nodeDataRaw : nodeArray.as_array()) {
    toml::table nodeData = nodeDataRaw.as_table();
    size_t index = nodeData["index"].as_integer(std::nothrow);
    Node &node = Nodes.at(index - 1);
    node.type = stringToNodeType(nodeData["type"].as_string(std::nothrow));
    node.position.x() = nodeData["x"].as_floating(std::nothrow);
    node.position.y() = nodeData["y"].as_floating(std::nothrow);
    node.cForce.x() = nodeData["cForceX"].as_floating(std::nothrow);
    node.cForce.y() = nodeData["cForceY"].as_floating(std::nothrow);
    node.cMoment = nodeData["cMoment"].as_floating(std::nothrow);
    for (toml::value connIndex : nodeData["connections"].as_array(std::nothrow)) {
      Connections.at(connIndex.as_integer() - 1).at(index - 1) = 1;
      Connections.at(index - 1).at(connIndex.as_integer() - 1) = 1;
    }
  }
}
