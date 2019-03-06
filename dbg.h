//
//  dbg.h
//  DNA组装
//
//  Created by mov on 2018/6/22.
//  Copyright © 2018 万俊鹏. All rights reserved.
//
#ifndef dbg_h
#define dbg_h

#include <iostream>
#include <algorithm>
#include <queue>
#include <map>
#include "configs.h"
#include "node.h"
#include "basic_functions.h"

map<struct node*, long, cmpKey> rbtree_list;

void update_length(struct node *Node){
    Node->flag = PASSING;
    if(Node->parents.size()<=0)
        return;
    for(auto it = Node->parents.begin(); it!= Node->parents.end(); it++){
        struct node * parent = *it;
        long tmp_len = Node->length + 1; //加入节点后，应该的长度
        if(parent->flag !=NORMAL && tmp_len > parent->length){
            //继续向上更新父节点的length
            parent->length = tmp_len;
            update_length(parent);
        }
    }
    Node->flag = NORMAL;
}

//构建图
vector<struct node*> build_dbg(vector<string> reads, int k, int short_length){
    vector<struct node*> node_list;      //记录节点的指针
    map<string, struct node*> mark_tree; //标记节点是否已经被创建过
    
    // 将每个read打碎成k-mer,插入dbg图中,同时维护length
    for(auto it=reads.begin(); it!=reads.end();it++){
        string read = *it;
        string f_kmer ="null";
        struct node* Father = nullptr;
        
        for(int i=0; i< short_length-k+1 ;i++){
            string kmer = read.substr(i, k);
            
            struct node* Node; //kmer对应的节点
            if(mark_tree.find(kmer)==mark_tree.end()){
                Node= new node;
                Node->kmer = kmer;
                Node->length = kmer.size();
                node_list.push_back(Node);
                mark_tree[kmer] = Node;
                Node->flag = NORMAL;
            }else{
                Node = mark_tree.find(kmer)->second;
            }
            
            if(f_kmer!="null" && Node_not_in(Node->parents, Father)){
                Node->parents.push_back(Father);
                push_back_length(Father->children, Node);
                Father->length = max_length(Father);
                update_length(Father);
            }
            
            Node->coverage +=1;
            f_kmer = kmer;
            Father = Node;
        }
    }
    cout<<"dgb graph builded"<<endl;
    return node_list;
}

/*
 父节点递归-1, 需要动态调整在红黑树中的位置；
 */
void substract_length(struct node* Node){
    for(auto it = Node->parents.begin(); it!= Node->parents.end(); it++){
        struct node* parent = *it;
        long length = max_length(parent);
        if(parent->length > length){
            rbtree_list.erase(parent);
            parent->length = length;
            rbtree_list[parent] = length;
            substract_length(parent);
        }
    }
}

//将节点从红黑树中去掉，并且递归减父节点length
void decrease_node(struct node* Node){
    rbtree_list.erase(Node);
    Node->length = 0;
    substract_length(Node);
    for(auto it = Node->parents.begin(); it!= Node->parents.end(); it++)
        (*it)->children.erase(remove((*it)->children.begin(),(*it)->children.end(), Node),(*it)->children.end());
    vector<struct node*> empty_vec;
    Node->parents = empty_vec;
}

////对于到来的父节点删除, 也不必更新其它父节点的值
//void decrease_node(struct node* Node, struct node* father){
//    if(!father) return;
////    Node->coverage -= dup_time;
//
//    erase_Node(father->children, Node);
//    erase_Node(Node->parents, father);
//    father->length = max_length(father);
//    substract_length(father);
//}
//
//void decrease_coverage(struct node* Node){
//    Node->coverage -= dup_time;
//    //Node->flag =
//}
//找路
vector<string> find_contig(struct node* travel_node){
    vector<string> contig;
//    struct node* Father = nullptr;
    while(true){
        contig.push_back(travel_node->kmer);
//        if(travel_node->coverage > COVERAGE_RATE){
//            decrease_coverage(travel_node);
//            if(Father)
//                erase_Node(travel_node->parents, Father);
////            if(travel_node->parents.size()==1)
////                //仅仅减其覆盖率
////            else{
////                decrease_node(travel_node, Father);
////            }
////            if(travel_node->children.size() < 1)
////                break;
////            cout<<travel_node->kmer<<endl;
////            struct node* children_node = travel_node->children[0];
////            travel_node = children_node;
////            continue;
//        }
//        else
        decrease_node(travel_node); //会保证没有父节点能够再次指向它
        
        if(travel_node->children.size() < 1)
            break;
        //取length最大的第一个孩子
        struct node* children_node = travel_node->children[0];
        
//        if(children_node->coverage>40) {travel_node = children_node;continue;}
        travel_node->children.erase(remove(travel_node->children.begin(),travel_node->children.end(), children_node),travel_node->children.end());
        children_node->parents.erase(remove(children_node->parents.begin(),children_node->parents.end(), travel_node), children_node->parents.end());
        
//        Father = travel_node;
        travel_node = children_node;
    }
    return contig;
}

vector<string> find_rb_contigs(vector<struct node*> node_list){
    vector<vector<string> > contigs;
    //按length构建红黑树
    for(auto it = node_list.begin(); it != node_list.end();it++)
        rbtree_list[(*it)] = (*it)->length;
    
    while(rbtree_list.size()>0){
        vector<string> contig;
        //取length最大的顶点
        struct node* travel_node = rbtree_list.begin()->first;
        contig = find_contig(travel_node);
        contigs.push_back(contig);
        if(contig.size()>6000){
            cout<<"congtig size:"<<endl;
            cout<<contig.size()<<endl;
        }
    }
    // 将找到的contigs转化成字符串
    vector<string> result;
    for(auto it = contigs.begin(); it!=contigs.end(); it++)
        result.push_back(contig2string(*it));
    return result;
}

struct node* combine_node(struct node* destination, struct node* source){
//    cout<<"combining bubble"<<endl;
    
    /*合并父亲节点, 同时动态调整length*/
    for(auto it= source->parents.begin(); it!=source->parents.end(); it++){
        struct node* parent = *it;
        if(Node_not_in(destination->parents,parent)){
            destination->parents.push_back(parent);
            push_back_length(parent->children, destination);
            //length 在buble的情况下是相同的，暂时不用管它；
//            long max_len = max_length(parent);
//            if(max_len> destination->length){
//                destination->length = max_len;
//                update_length(parent);
        }
        erase_Node(parent->children, source);
    }
    
    /*合并子节点, 同时动态调整length*/
    for(auto it=source->children.begin(); it!=source->children.end(); it++){
        struct node* child = *it;
        if(Node_not_in(destination->children, child)){
            destination->children.push_back(child);
            push_back_length(child->parents, destination);
        }
        erase_Node(child->parents, source);
    }
    
    vector<struct node* > empty_vec;
    source->parents =  empty_vec;
    source->children=  empty_vec;
    //源节点标记为bubble;--> 并且，已经没有儿子或父亲指针指向它;
    destination->coverage += source->coverage;  //叠加重复率
    source->flag =BUBBLE;
    return destination;
}

void handle_bubble(struct node* Node){
    for(int i=0; i<Node->children.size(); i++)
        for(int j=i+1; j<Node->children.size(); j++){
            struct node* a = Node->children[i];
            struct node* b = Node->children[j];
            int distance = dis_rate(a, b);
            if(distance < DIS_THRESHOLD && a->flag == NORMAL &&b->flag ==NORMAL ){
                int covrage_a = a->coverage, covrage_b = b->coverage;
                struct node* destination;
//                cout<<distance<<endl;
//                cout<<a->kmer<<endl;
//                cout<<b->kmer<<endl;
                if(covrage_a > covrage_b)
                    destination= combine_node(a,b);
                else
                    destination= combine_node(b,a);
                if(destination->flag==NORMAL && destination->children.size()>=2){
                    handle_bubble(destination);
                }
                i=j=0; //无法合并时, 循环会自动退出
            }
        }
}

void delete_bubbles(vector<struct node*> node_list){
    for(auto it= node_list.begin(); it!= node_list.end(); it++){
        struct node* Node = *it;
        if(Node->flag==NORMAL && Node->children.size()>=2)
            handle_bubble(Node);
    }
    cout<<"bubble deleted"<<endl;
}

//定义全局变量，解螺旋之后的点的集合
vector<struct node*> unbind_list;

int repeat_nums = 0;
//其子节点与对应的分裂节点建立关系
void build_ship(vector<struct node*> Nodes, struct node* child){
    int cov = child->coverage;
    long s=99999999;
    struct node* Node = nullptr;
    for(auto it = Nodes.begin(); it!= Nodes.end(); it++)
        if(abs(cov - (*it)->coverage)<s){
            s =abs(cov - (*it)->coverage);
            Node = *it;
        }
    push_back_length(Node->children, child);
    child->parents.clear();
    child->parents.push_back(Node);
    child->coverage = Node->coverage;
}

void handle_repeat(struct node* Node){
    repeat_nums++;
    //将parent按照covrage升序排列
    vector<struct node*> children = Node->children;
    Node->children.clear();
    
    vector<struct node*> Nodes;
    Nodes.push_back(Node);
    //每一个父节点对应一个Node的复制品
    for(auto iter = Node->parents.begin(); iter!=Node->parents.end();iter++){
        if(iter == Node->parents.end()) {
            Node->coverage = (*iter)->coverage;
            Node->parents.clear();
            Node->parents.push_back(*iter);
            continue;
        }
        struct node* parent = *iter;
        struct node* Node2 =  new node;
        Node2->coverage = parent->coverage;
        Node2->kmer = Node->kmer;
        Node2->length = Node->length;
        
        erase_Node(parent->children, Node);
        push_back_length(parent->children, Node2);
        Node2->parents.push_back(parent);
        unbind_list.push_back(Node2);
        Nodes.push_back(Node2);
    }
    //每一个子节点依次与频率最近的Node建立联系
    if(children.size()==1){
        for(auto it = Nodes.begin(); it!= Nodes.end(); it++){
            push_back_length((*it)->children, children[0]);
            children[0]->parents.push_back(*it);
        }
//        cout<<"flag"<<endl;
        if(children[0]->flag==NORMAL && children[0]->parents.size()>=2 && children[0]->coverage > COVERAGE_RATE)
            handle_repeat(children[0]);
    }else{
        for(auto it = children.begin(); it!= children.end(); it++)
             build_ship(Nodes, *it);
//        cout<<"fffffff"<<endl;
        //对于每一个node，更新其length
        for(auto it = Nodes.begin(); it!= Nodes.end(); it++){
            (*it)->length = max_length(*it);
            update_length(*it);
        }
    }
}

vector<struct node*> unbind_repeat(vector<struct node*> node_list){
    unbind_list = node_list;
    
    for(auto it= node_list.begin(); it!= node_list.end(); it++){
        struct node* Node = *it;
        if(Node->flag==NORMAL && Node->parents.size()>=2 && Node->coverage>COVERAGE_RATE)
            handle_repeat(Node);
    }
    
    cout<<"repeated nums"<<endl;
    cout<<repeat_nums<<endl;
    return unbind_list;
}


void release_dbg(vector<struct node*> node_list){
    for(auto it = node_list.begin(); it!= node_list.end();it++ )
        delete *it;
    cout<<"dbg graph released"<<endl;
}

#endif /* dbg_h */
