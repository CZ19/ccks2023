// 目前的最终版本-提交
int n=queryUtil->getVertNum();
map<int,int> node_label;
map<int,set<int>> node_ins;
map<int,map<int,int>> label_counts;
for(int i=0;i<n;i++){
    node_label[i]=i;
}


if(!directed){
    for(int kk=0;kk<k;kk++){
        int flag=0;
        for(int i=0;i<n;i++){
            map<int,int> label_counts2;
            for(auto& pred:pred_set){
                // map<int,int> label_counts;
                set<int> used;
                used.insert(i);

                int out_size=queryUtil->getOutSize(i,pred);
                int in_size=queryUtil->getInSize(i,pred);

                for(int in_count=0;in_count<in_size;in_count++){
                    int node=queryUtil->getInVertID(i,pred,in_count);
                    if(used.find(node)!=used.end())continue;
                    used.insert(node);
                    if(label_counts2.find(node_label[node])==label_counts2.end())label_counts2[node_label[node]]=1;
                    else label_counts2[node_label[node]]+=1;
                }
                
                
                for(int out_count=0;out_count<out_size;out_count++){
                    int node=queryUtil->getOutVertID(i,pred,out_count);
                    if(used.find(node)!=used.end())continue;
                    used.insert(node);
                    if(label_counts2.find(node_label[node])==label_counts2.end())label_counts2[node_label[node]]=1;
                    else label_counts2[node_label[node]]+=1;
                }
            }

            int max_label=-1;
            int max_count=0;
            for(auto& label_count:label_counts2){
                if(label_count.second>max_count){
                    max_count=label_count.second;
                    max_label=label_count.first;
                }
            }
            if(max_label!=node_label[i]&&max_label!=-1){
                node_label[i]=max_label;
                flag=1;
            }
        }
        if(flag==0)break;
    }
}else{
        for(int i=0;i<n;i++){
            set<int> used;
            used.insert(i);
            for(auto& pred:pred_set){
                int in_size=queryUtil->getInSize(i,pred);
                for(int in_count=0;in_count<in_size;in_count++){
                    int node=queryUtil->getInVertID(i,pred,in_count);
                    if(used.find(node)!=used.end())continue;
                    used.insert(node);
                    if(label_counts[i].find(node_label[node])==label_counts[i].end())label_counts[i][node_label[node]]=1;
                    else label_counts[i][node_label[node]]+=1;
                    node_ins[node].insert(i);
                }
            }
            used.clear();
        }      
        for(int kk=0;kk<k;kk++){
        int flag=0;
        for(int i=0;i<n;i++){
            int max_label=-1;
            int max_count=0;
            for(auto& label_count:label_counts[i]){
                if(label_count.second>max_count){
                    max_count=label_count.second;
                    max_label=label_count.first;
                }
            }
            if(max_label!=node_label[i]&&max_label!=-1){
                flag=1;
                for(auto& node_in:node_ins[i]){
                    label_counts[node_in][node_label[i]]-=1;
                    if(label_counts[node_in].find(max_label)==label_counts[node_in].end())label_counts[node_in][max_label]=1;
                    else label_counts[node_in][max_label]++;
                }
                node_label[i]=max_label;    
            }
        }
        if(flag==0)break;
    }
}

map<int,vector<int>> res;
for(auto& node_labelx:node_label){
    res[node_labelx.second].push_back(node_labelx.first);
}
stringstream ss;
int res_len=res.size();

ss<<"[";

int res_index=1;
for(auto& re:res){
    ss<<"[";
    ss<<queryUtil->getPathString(re.second);
    ss<<"]";
    if(res_index++!=res_len)ss<<",";
}
ss<<"]";


return ss.str();