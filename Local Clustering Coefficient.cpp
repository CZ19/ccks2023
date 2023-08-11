#include "../../Query/PathQueryHandler.h"
 
using namespace std;

PathQueryHandler* apiUtil;

vector<int> cal_neighbor(int uid, std::vector<int> pred_set)
{
    std::map<int,int> u_nei;
    std::vector<int> neighbor;//uid的入边邻居节点
    for(int i=0;i<pred_set.size();i++)
    {
        int num=apiUtil->getInSize(uid,pred_set[i]);//获取结点u以集合中的谓词为标签的入边所关联的邻居数（不考虑去重）
        for (int j=0;j<num;j++)
        {
            int tmp=apiUtil-> getInVertID(uid,pred_set[i],j);
            neighbor.push_back(tmp);//获取uid的入边邻居节点
            u_nei[tmp]=1;
        }
    }
    for(int i=0;i<pred_set.size();i++)
    {
        int num=apiUtil->getOutSize(uid,pred_set[i]);//获取结点u以集合中的谓词为标签的出边所关联的邻居数（不考虑去重）
        for (int j=0;j<num;j++)
        {
            int tmp=apiUtil-> getOutVertID(uid,pred_set[i],j);
            if(!u_nei[tmp])
            {
                neighbor.push_back(tmp);//获取uid的出边邻居节点
                u_nei[tmp]=1;
            }
        }
    }
    return neighbor;
}

float clustercoeff_Ndirected(int uid, std::vector<int> pred_set)
{
    std::map<int, int> u_nei;
    std::map<pair<int, int>, int> u_v;
    std::vector<int> uneighbor;
    stringstream ss;
    uneighbor = cal_neighbor(uid,pred_set);
    for (int i = 0; i < uneighbor.size(); i++)
    {
        u_nei[uneighbor[i]] = 1;
    }
    int up = 0;
    for (int i = 0; i < uneighbor.size(); i++)
    {
        int vid = uneighbor[i];
        std::vector<int> vneighbor;
        vneighbor = cal_neighbor(vid,pred_set);
        for (int j = 0; j < vneighbor.size(); j++)
        {
            if (u_nei.find(vneighbor[j]) != u_nei.end() && u_v.find(std::make_pair(vid, vneighbor[j])) == u_v.end())
            {
                u_v[std::make_pair(vid, vneighbor[j])] = 1;
                u_v[std::make_pair(vneighbor[j], vid)] = 1;
                up++;
            }
        }
    }
    int uneighbor_num = uneighbor.size();
    float bottom = uneighbor_num * (uneighbor_num - 1);
    float ans = 2 * up / bottom;
    return ans;
}
float clustercoeff_directed(int uid, std::vector<int> pred_set)
{

set<int> in_nodes;
set<int> out_nodes;
for(auto pred:pred_set){
    int in_size=apiUtil->getInSize(uid,pred);
    int out_size=apiUtil->getOutSize(uid,pred);
    for(int in_index=0;in_index<in_size;++in_index){
        int node=apiUtil->getInVertID(uid, pred, in_index);
        in_nodes.insert(node);
    }
    for(int out_index=0;out_index<out_size;++out_index){
        int node=apiUtil->getOutVertID(uid, pred, out_index);
        out_nodes.insert(node);
    }
}

//剔除uid
if(in_nodes.find(uid)!=in_nodes.end())in_nodes.erase(uid);
if(out_nodes.find(uid)!=out_nodes.end())out_nodes.erase(uid);

int in_node_size=in_nodes.size();
int out_node_size=out_nodes.size();
float double_node_size=0;
float out_in_num=0;



for( auto& out_node:out_nodes){
    for(auto& pred:pred_set){
        int out_size=apiUtil->getOutSize(out_node,pred);
        for(int out_index=0;out_index<out_size;++out_index){
            int out_out_node=apiUtil->getOutVertID(out_node, pred, out_index);
            if(in_nodes.find(out_out_node)!=in_nodes.end()&&out_out_node!=out_node){
                out_in_num+=1;
            }
        }
    }
    if(in_nodes.find(out_node)!=in_nodes.end())double_node_size+=1;
}

float sum_num=in_node_size*out_node_size-double_node_size;
float ans=out_in_num/sum_num;


return ans;

}



extern "C" string ccks_clustercoeff_65ae3ced83084582bd2052d1764e15fa(std::vector<int> iri_set, bool directed, std::vector<int> pred_set, PathQueryHandler* queryUtil)
{
    apiUtil=queryUtil;
    stringstream ss;
    for(int i=0;i<iri_set.size();i++)
    {
        int uid=iri_set[i];
        if (i != 0)
        {
            ss << ",";
        }
        if(!directed)
            ss<<clustercoeff_Ndirected(uid,pred_set);
        else{
            ss<<clustercoeff_directed(uid,pred_set);

        }
    }
    return ss.str();
}