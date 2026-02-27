#ifndef TOPOLOGICAL_SORT_H
#define TOPOLOGICAL_SORT_H

#include <vector>
#include <queue>

std::vector<int> topological_sort(std::vector<std::vector<int>> adjacency_list);
std::vector<int> topological_sort_w_nodes(std::vector<std::vector<int>> adjacency_list, int n_nodes);

#endif
