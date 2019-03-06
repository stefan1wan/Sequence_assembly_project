//
//  node.h
//  DNA组装
//
//  Created by mov on 2018/6/24.
//  Copyright © 2018 万俊鹏. All rights reserved.
//

#ifndef node_h
#define node_h

#define PASSING 0
#define NORMAL 1
#define JOINED 2
#define BUBBLE 3
#include <vector>

using namespace std;

struct node{
    string kmer;
    vector<struct node*> parents;
    vector<struct node*> children;
    long length;
    int coverage;
    int flag;
    node():length(0),coverage(0), flag(0){}
    bool operator < (const node a) const  //重载 < 运算符自定义排序准则；
    {
        return a.length > length;
    }
};

//在构建红黑树的时候，需要的比较函数
struct cmpKey{
    bool  operator() ( const node*  key1, const node*  key2) const{
        if(key1->length== key2->length)
            return key1->kmer < key2->kmer;
        
        return key1->length> key2->length;
    }
};

bool Less(node* & s1, node* & s2){
    return s1->length < s2->length; //从小到大排序
}

bool Greater(node* & s1, node* & s2){
    return s1->length > s2->length; //从大到小排序
}

bool Less_cov(node* & s1, node* & s2){
    return s1->coverage < s2->coverage; //从小到大排序
}

/*不相似度*/
int dis_rate(struct node* a, struct node* b){
    string kmer_a = a->kmer, kmer_b = b->kmer;
    int rate = 0 ;
    for(int i =0; i<kmer_a.size(); i++){
        if(kmer_a[i]!= kmer_b[i])
            rate+=1;
    }
    return rate;
}

//最大长度为最大子节点长度+1
long max_length(struct node *Node){
    long max_len =0;
    for(auto it=Node->children.begin(); it!=Node->children.end(); it++)
        if((*it)->length > max_len) max_len = (*it)->length;
    if(max_len >0)
        max_len = max_len+1;
    else
        max_len = Node->kmer.size();
    return max_len;
}

//判断一个元素是否在向量中
bool Node_not_in(vector<struct node*> vec, struct node* Node){
    for(auto it = vec.begin(); it!= vec.end(); it++)
        if((*it)==Node)
            return false;
    return true;
}
//删除向量中某个元素
void erase_Node(vector<struct node*>& vec, struct node* Node){
    vec.erase(remove(vec.begin(),vec.end(), Node),vec.end());
}

//按长度降序，将节点放入向量中
void push_back_length(vector<struct node*>& vec, struct node* Node){
    if(vec.size()==0){
        vec.push_back(Node);
        return;
    }
    long len_t = vec.size();
    for(auto it = vec.begin(); it!=vec.end(); it++)
        if((*it)->length < Node->length){
            vec.insert(it,Node);
            break;
        }
    if(len_t == vec.size())
        vec.push_back(Node);
}
#endif /* node_h */
