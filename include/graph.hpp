#pragma once
#include <vector>

template <typename T> class Graph {
public:
  Graph() : node_count(0) {}

  T& at(std::size_t start, std::size_t end) { return data[start][end]; }

  std::size_t GetNodeCount() const { return node_count; }

  void Resize(std::size_t _node_count) {
    node_count = _node_count;
    data.resize(_node_count);
    for (std::vector<T>& row : data) {
      row.resize(_node_count);
    }
  }

private:
  std::size_t node_count;
  std::vector<std::vector<T>> data;
};
