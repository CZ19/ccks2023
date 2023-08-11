#include "../../Query/PathQueryHandler.h"

using namespace std;

PathQueryHandler* apiUtil;

// 执行时长29-提交

vector<int> shortestPath(int uid,int vid, bool directed, std::vector<int> pred_set) //cost less space and less time.
{
	vector<int> ans;
    unordered_map<int,int> father_u;
    unordered_map<int,int> son_v;
    unordered_map<int,int> labels;
    father_u[uid]=uid;
    son_v[vid]=vid;
    std::queue<int> q_u, q_v;
	q_u.push(uid);
	q_v.push(vid);
	while(!q_u.empty() || !q_v.empty()){
        std::queue<int> new_q_u;
        std::queue<int> new_q_v;
        while(!q_u.empty()){
            int temp=q_u.front();
            q_u.pop();
            for(auto pred:pred_set){
                int out_size=apiUtil->getOutSize(temp, pred);
                for(int out_index=0;out_index<out_size;++out_index){
                    int node=apiUtil->getOutVertID(temp, pred, out_index);
                    if(son_v.find(node)!=son_v.end()){
                        //找到meet_node
                        ans.push_back(node);
                        ans.push_back(pred);
                        ans.push_back(temp);
                        int temp_u=temp;
                        while (temp_u!=uid)
                        {
                            ans.push_back(labels[temp_u]);
                            temp_u=father_u[temp_u];
                            ans.push_back(temp_u);
                        }
                        std::reverse(ans.begin(),ans.end());
                        int temp_v=node;
                        while(temp_v!=vid){
                            ans.push_back(labels[temp_v]);
                            temp_v=son_v[temp_v];
                            ans.push_back(temp_v);
                        }
                        return ans;
                    }
                    if(father_u.find(node)==father_u.end()){
                        //处理node
                        new_q_u.push(node);
                        father_u[node]=temp;
                        labels[node]=pred;
                    }

                }
                
                if(directed)continue;
                
                int in_size=apiUtil->getInSize(temp, pred);
                for(int in_index=0;in_index<in_size;++in_index){
                    int node=apiUtil->getInVertID(temp, pred, in_index);
                    if(son_v.find(node)!=son_v.end()){
                        //找到meet_node
                        ans.push_back(node);
                        ans.push_back(pred);
                        ans.push_back(temp);
                        int temp_u=temp;
                        while (temp_u!=uid)
                        {
                            ans.push_back(labels[temp_u]);
                            temp_u=father_u[temp_u];
                            ans.push_back(temp_u);
                        }
                        std::reverse(ans.begin(),ans.end());
                        int temp_v=node;
                        while(temp_v!=vid){
                            ans.push_back(labels[temp_v]);
                            temp_v=son_v[temp_v];
                            ans.push_back(temp_v);
                        }
                        return ans;
                    }
                    if(father_u.find(node)==father_u.end()){
                        //处理node
                        new_q_u.push(node);
                        father_u[node]=temp;
                        labels[node]=pred;
                    }
                }
            }
        }
        q_u=new_q_u;
        while (!q_v.empty()){
            int temp=q_v.front();
            q_v.pop();
            for(auto pred:pred_set){
                int in_size=apiUtil->getInSize(temp,pred);
                for(int in_index=0;in_index<in_size;++in_index){
                    int node=apiUtil->getInVertID(temp,pred,in_index);
                    if(father_u.find(node)!=father_u.end()){
                        //找到meet_node
                        ans.push_back(node);
                        int temp_u=node;
                        while (temp_u!=uid)
                        {
                            ans.push_back(labels[temp_u]);
                            temp_u=father_u[temp_u];
                            ans.push_back(temp_u);
                        }
                        std::reverse(ans.begin(),ans.end());
                        ans.push_back(pred);
                        ans.push_back(temp);
                        int temp_v=temp;
                        while(temp_v!=vid){
                            ans.push_back(labels[temp_v]);
                            temp_v=son_v[temp_v];
                            ans.push_back(temp_v);
                        }
                        return ans;
                    }
                    if(son_v.find(node)==son_v.end()){
                        new_q_v.push(node);
                        son_v[node]=temp;
                        labels[node]=pred;
                    }
                }

                if(directed)continue;

                int out_size=apiUtil->getOutSize(temp,pred);
                for(int out_index=0;out_index<out_size;++out_index){
                    int node=apiUtil->getOutVertID(temp,pred,out_index);
                    if(father_u.find(node)!=father_u.end()){
                        //找到meet_node
                        ans.push_back(node);
                        int temp_u=node;
                        while (temp_u!=uid)
                        {
                            ans.push_back(labels[temp_u]);
                            temp_u=father_u[temp_u];
                            ans.push_back(temp_u);
                        }
                        std::reverse(ans.begin(),ans.end());
                        ans.push_back(pred);
                        ans.push_back(temp);
                        int temp_v=temp;
                        while(temp_v!=vid){
                            ans.push_back(labels[temp_v]);
                            temp_v=son_v[temp_v];
                            ans.push_back(temp_v);
                        }
                        return ans;
                    }
                    if(son_v.find(node)==son_v.end()){
                        new_q_v.push(node);
                        son_v[node]=temp;
                        labels[node]=pred;
                    }
                }
            }
        }
        q_v=new_q_v;
        
    }
	return ans;
}

extern "C" string ccks_cyclepath_65ae3ced83084582bd2052d1764e15fa(std::vector<int> iri_set, bool directed, std::vector<int> pred_set, PathQueryHandler* queryUtil)
{
    apiUtil=queryUtil;
    std::vector<int> ans;
    int uid=iri_set[0];
    int vid=iri_set[1];
	stringstream ss;
	if(directed)
	{
		vector<int> ans1, ans2;
		ans1 = shortestPath(uid,vid, true, pred_set);
		ans2 = shortestPath(vid,uid, true, pred_set);
		if(ans1.size() == 0 || ans2.size() == 0)
        {
			return apiUtil->getPathString(ans);
        }
		else
		{
			int ans2_size=ans2.size() - 1;
            for(int i = 1; i < ans2_size; ++i)
			ans1.push_back(ans2[i]);
			ans1.push_back(uid);
			return apiUtil->getPathString(ans1);
		}
	}
    else
    {
        ans = shortestPath(uid,vid, false, pred_set);
		int lens = ans.size();
		if (lens == 0)
		{
			return apiUtil->getPathString(ans);
        }
		for(int i = lens - 2; i > 0; --i)
		{
            ans.push_back(ans[i]);
        }
		ans.push_back(uid);
        return apiUtil->getPathString(ans);
    }
}