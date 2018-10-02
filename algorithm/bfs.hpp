#ifndef BFS_HPP
#define BFS_HPP

#include <vector>
#include <queue>

// graph - Adjacency list
void bfs(std::vector<std::vector<size_t>> const& graph, size_t start_v = 0) {
    std::vector<bool> visit(graph.size(), false);

    std::queue<size_t> q;

    visit[start_v] = true;
    q.push(start_v);

    while(!q.empty()) {
        size_t v = q.front();
        q.pop();

        for(auto &&to : graph[v]) {
            if(visit[to]) {
                continue;
            }

            // visiting v->to
            // something...

            visit[to] = true;
            q.push(to);
        }
    }
}

#endif // BFS_HPP
