#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <sstream>
#include "../../Query/PathQueryHandler.h"

using namespace std;

extern "C" string ccks_labelprop_65ae3ced83084582bd2052d1764e15fa(std::vector<int> iri_set, bool directed, int k, std::vector<int> pred_set, PathQueryHandler* queryUtil) {
    int num = queryUtil->getVertNum();
    stringstream ss;
    ss << "[";

    vector<vector<int>> adjacencyList(num); // 邻接表，存储每个节点的邻居节点
    vector<bool> visited(num, false); // 记录节点的访问状态

    // 构建邻接表
    for (int i = 0; i < num; i++) {
        for (auto pred : pred_set) {
            int out_size = queryUtil->getOutSize(i, pred);
            for (int j = 0; j < out_size; j++) {
                int out_id = queryUtil->getOutVertID(i, pred, j);
                adjacencyList[i].push_back(out_id);
                adjacencyList[out_id].push_back(i);
            }
        }
    }

    unordered_map<int, vector<int>> res; // 存储每个节点的弱联通分量

    for (int i = 0; i < num; i++) {
        if (visited[i]) continue;
        vector<int> component; // 弱联通分量中的节点
        dfs(i, adjacencyList, visited, component);
        res[i] = move(component);
    }

    // 输出结果
    int res_len = res.size();
    int res_index = 1;
    for (auto& entry : res) {
        int node = entry.first;
        auto& component = entry.second;
        ss << "[";
        ss << queryUtil->getPathString(component);
        ss << "]";
        if (res_index++ != res_len) ss << ",";
    }
    ss << "]";

    return ss.str();
}

// 深度优先搜索（DFS）遍历节点的弱联通分量
void dfs(int node, const vector<vector<int>>& adjacencyList, vector<bool>& visited, vector<int>& component) {
    visited[node] = true;
    component.push_back(node);
    for (int neighbor : adjacencyList[node]) {
        if (!visited[neighbor]) {
            dfs(neighbor, adjacencyList, visited, component);
        }
    }
}
