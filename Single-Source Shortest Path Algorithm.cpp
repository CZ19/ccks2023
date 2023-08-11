#include "../../Query/PathQueryHandler.h"

using namespace std;

extern "C" string ccks_sssp_65ae3ced83084582bd2052d1764e15fa(std::vector<int> iri_set, bool directed, std::vector<int> pred_set, PathQueryHandler* queryUtil)
{
std::unordered_map<int, int> ans;
std::queue<int> visiting;

int uid = iri_set[0];//开始的节点
int cur = -1;
int num_out = -1;
int out_id = -1;
ans[uid] = 0;
visiting.push(uid);
while (!visiting.empty()) {//BFS单源最短路径
    cur = visiting.front();//当前访问的节点
    visiting.pop();
    for (auto pred : pred_set)
    {
        num_out = queryUtil->getOutSize(cur, pred);//节点出边的数量
        for (int i = 0; i < num_out; i++) {
            out_id = queryUtil->getOutVertID(cur, pred, i);//遍历节点的每一个邻居
            if (ans.find(out_id) == ans.end()) {//判断这个邻居有没有被访问过
                visiting.push(out_id);//没有被访问加入队列
                ans[out_id] = ans[cur] + 1;//路径为父节点+1
            }
        }
        if (directed) continue;

        num_out = queryUtil->getInSize(cur, pred);//节点出边的数量
        for (int i = 0; i < num_out; i++) {
            out_id = queryUtil->getInVertID(cur, pred, i);//遍历节点的每一个邻居
            if (ans.find(out_id) == ans.end()) {//判断这个邻居有没有被访问过
                visiting.push(out_id);//没有被访问加入队列
                ans[out_id] = ans[cur] + 1;//路径为父节点+1
            }
        }


    }

}

// 使用 stringstream 构建结果字符串
std::stringstream ss;
ss << "[";
auto it = ans.begin();
if (it != ans.end()) {
    ss << "{\"dst\":\"" << it->first << "\",\"value\":\"" << it->second << "\"}";
    ++it;
    for (; it != ans.end(); ++it) {
        ss << ",{\"dst\":\"" << it->first << "\",\"value\":\"" << it->second << "\"}";
    }
}
ss << "]";

return ss.str();

}