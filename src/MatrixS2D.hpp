#pragma once
#include <vector>

template <typename T> class MatrixS2D {
public:
  MatrixS2D() : size(0) {}

  T& at(std::size_t start, std::size_t end) { return data.at(start).at(end); }

  std::size_t GetNodeCount() const { return size; }

  void Resize(std::size_t _size) {
    size = _size;
    data.resize(_size);
    for (std::vector<T>& row : data) {
      row.resize(_size);
    }
  }

  void Remove(std::size_t index) {
    data.erase(data.begin() + index);
    for (std::vector<T>& row : data) {
      row.erase(row.begin() + index);
    }
  }

private:
  std::size_t size;
  std::vector<std::vector<T>> data;
};
