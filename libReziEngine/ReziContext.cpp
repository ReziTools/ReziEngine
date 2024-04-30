#include "ReziContext.hpp"
#include <fstream>
#include <iostream>

size_t ReziContext::GetNodeCount(void) const { return Nodes.size(); }

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

void ReziContext::SaveReziCode(const std::filesystem::path path, std::string &err) const {
  err.clear();
  if (!GetNodeCount()) {
    err = "Context has no nodes.";
    return;
  }
  std::ofstream fout(path.string());
  if (!fout.is_open()) {
    err = "File is not writable!";
    return;
  }
  for (size_t i = 0; i < GetNodeCount(); i++) {
    const Node &node = Nodes.at(i);
    fout << "node[" << i + 1 << "," << NodeTypeNamesRezi.at(node.type)
         << "]: " << node.position.x() << " " << node.position.y() << "\n";
  }
  for (size_t i = 0; i < GetNodeCount(); i++) {
    fout << "conn[" << i + 1 << "]: ";
    for (size_t j = 0; j < GetNodeCount(); j++) {
      if (Connections.at(i).at(j))
        fout << j + 1 << " ";
    }
    fout << "\n";
  }
  for (size_t i = 0; i < GetNodeCount(); i++) {
    const Node &node = Nodes.at(i);
    if (node.cForce.norm() > LOW_CUTOFF)
      fout << "cforce[" << i + 1 << "]: " << node.cForce.x() << " " << node.cForce.y() << "\n";
  }
  for (size_t i = 0; i < GetNodeCount(); i++) {
    const Node &node = Nodes.at(i);
    if (node.cMoment > LOW_CUTOFF)
      fout << "cmoment[" << i + 1 << "]: " << node.cMoment << "\n";
  }
  fout.close();
}
