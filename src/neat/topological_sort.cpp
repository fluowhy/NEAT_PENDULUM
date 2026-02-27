#include "topological_sort.h"
#include <vector>
#include <queue>

std::vector<int> topological_sort(std::vector<std::vector<int>> adjacency_list){
    int n { static_cast<int>(adjacency_list.size()) };
    std::vector<int> result {};
    std::vector<int> indegree (n, 0);
    std::queue<int> queue {};

    for (std::vector<int>& edge : adjacency_list){
        ++indegree[edge[1]];
    }

    for (int i {0}; i<static_cast<int>(indegree.size()); ++i){
        if (indegree[i] == 0){
            queue.push(i);
        }
    }

    while (!queue.empty()){
        result.push_back(queue.front());
        queue.pop();
        for (std::vector<int>& edge : adjacency_list){
            if (result.back() == edge[0]){
                --indegree[edge[1]];
                if (indegree[edge[1]] == 0){
                    queue.push(edge[1]);
                }
            }
        }
    }
    return result;
}

std::vector<int> topological_sort_w_nodes(std::vector<std::vector<int>> adjacency_list, int n_nodes){
    std::vector<int> result {};
    std::vector<int> indegree (n_nodes, 0);
    std::queue<int> queue {};

    for (std::vector<int>& edge : adjacency_list){
        ++indegree[edge[1]];
    }

    for (int i {0}; i<static_cast<int>(indegree.size()); ++i){
        if (indegree[i] == 0){
            queue.push(i);
        }
    }

    while (!queue.empty()){
        result.push_back(queue.front());
        queue.pop();
        for (std::vector<int>& edge : adjacency_list){
            if (result.back() == edge[0]){
                --indegree[edge[1]];
                if (indegree[edge[1]] == 0){
                    queue.push(edge[1]);
                }
            }
        }
    }
    return result;
}
