#pragma once
#include "ReziContext.hpp"

namespace ReziSolver {
void SolveT(ReziContext &context);
}

template <typename T>
void DFS(const size_t node_index, const std::vector<std::vector<T>> &graph, std::vector<bool> &visited);

bool CheckContextDFS(const ReziContext &context);
bool CheckContextAlignX(const ReziContext &context);
bool CheckContextAlignY(const ReziContext &context);
size_t GetContextMomentMatrixCount(const ReziContext &context);
