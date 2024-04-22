#include "ReziContext.hpp"

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

void ReziContext::EmitReziCode(void) const {
  for (size_t i = 0; i < GetNodeCount(); i++) {
    const Node &node = Nodes.at(i);
    std::cout << "node[" << i << " " << (int)node.type
              << "]: " << node.position.x() << " " << node.position.y() << "\n";
  }
  for (size_t i = 0; i < GetNodeCount(); i++) {
    std::cout << "conn[" << i << "]: ";
    for (size_t j = 0; j < GetNodeCount(); j++) {
      if (Connections.at(i).at(j))
        std::cout << j << " ";
    }
    std::cout << "\n";
  }
  for (size_t i = 0; i < GetNodeCount(); i++) {
    const Node &node = Nodes.at(i);
    std::cout << "cforce[" << i << " "
              << "]: " << node.cForce.x() << " " << node.cForce.y() << "\n";
  }
  for (size_t i = 0; i < GetNodeCount(); i++) {
    const Node &node = Nodes.at(i);
    std::cout << "cmoment[" << i << " "
              << "]: " << node.cMoment << "\n";
  }
}
