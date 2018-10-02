#ifndef DFS_HPP
#define DFS_HPP

#include <vector>
#include <stack>

// graph - Adjacency list
void dfs(std::vector<std::vector<size_t>> const& graph, size_t start_v = 0) {
    std::vector<bool> visit(graph.size(), false);

    struct Node {
        size_t v;
        size_t idx_nei;
    };

    std::stack<Node> s;

    visit[start_v] = true;
    s.push({start_v, 0});

    while(!s.empty()) {
        auto &[v, idx] = s.top();

        for(; idx < graph[v].size(); ++idx) {
            if(!visit[idx]) {

                // visiting v->idx
                // something....

                visit[idx] = true;
                s.push({idx, 0});
                ++idx;
                break;
            }
        }
    }

}

#endif // DFS_HPP
