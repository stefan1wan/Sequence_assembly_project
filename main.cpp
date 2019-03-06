//
//  main.cpp
//  DNA组装
//
//  Created by mov on 2018/6/22.
//  Copyright © 2018 wjp. All rights reserved.
//

#include "dbg.h"

int main(int argc, const char * argv[]) {

    string genome = "ATCG";
    cout<<reverse_complement(genome)<<endl;
    vector<string> short_reads1 = read_fasta(SHORT_1);
    vector<string> short_reads2 = read_fasta(SHORT_2);
    vector<string> long_reads = read_fasta(LONG);

    vector<string> rc_string;
    for(auto it=short_reads1.begin(); it!= short_reads1.end(); it++){
        rc_string.push_back(reverse_complement(*it));
    }
    for(auto it=short_reads2.begin(); it!= short_reads2.end(); it++){
        rc_string.push_back(reverse_complement(*it));
    }
    rc_string.insert(rc_string.end(), short_reads1.begin(), short_reads1.end());
    rc_string.insert(rc_string.end(), short_reads2.begin(), short_reads2.end());

    vector<struct node*> node_list= build_dbg(rc_string, K, short_read_length);
    

    cout<<"the total node_nums are:"<<endl;
    cout<<node_list.size()<<endl;
    
    
//    for(auto it = node_list.begin(); it!= node_list.end(); it++){
//        cout<<(*it)->children.size()<<endl;
//    }
    delete_bubbles(node_list);
    

    vector<struct node*> correct_node_list;
    for(auto it = node_list.begin(); it!= node_list.end(); it++){
        if((*it)->flag==NORMAL)
            correct_node_list.push_back(*it);
    }
    
//    for(auto it = correct_node_list.begin(); it!= correct_node_list.end(); it++){
//        struct node* Node = *it;
//        if(Node->coverage>50)
//            cout<<Node->coverage<<endl;
//    }
    
    cout<<"the toal node_nums after deleted bubbles  are:"<<endl;
    cout<<correct_node_list.size()<<endl;
    
    correct_node_list = unbind_repeat(correct_node_list);
    
    cout<<"the toal node_nums after unbind repeats are:"<<endl;
    cout<<correct_node_list.size()<<endl;
    
    vector<string> contigs = find_rb_contigs(correct_node_list);

    write_fasta(RESULT_POS, contigs);
    
    return 0;
}
