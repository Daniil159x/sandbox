#ifndef DFS_HPP
#define DFS_HPP

#include <vector>
#include <stack>

// graph - список смежности, отсчёт с 0
void dfs(std::vector<std::vector<size_t>> const& graph, size_t start_v = 0) {
    std::vector<bool> visit(graph.size(), false);

    struct Node {
        size_t v;
        size_t idx_nei;
    };

    std::stack<Node> s;
    s.push({start_v, 0});

    while(!s.empty()) {
        auto &[v, idx] = s.top();

        for(; idx < graph[v].size(); ++idx) {
            if(!visit[idx]) {

                // посещение v->idx

                s.push({idx, 0});
                ++idx;
                break;
            }
        }
    }

}

#endif // DFS_HPP
