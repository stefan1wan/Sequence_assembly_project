//
//  basic_functions.h
//  DNA组装
//
//  Created by mov on 2018/6/22.
//  Copyright © 2018 万俊鹏. All rights reserved.
//

#ifndef basic_functions_h
#define basic_functions_h
#include <vector>
#include <fstream>
#include <string>
using namespace std;

/*
   获得序列的反向互补序列
 */
string reverse_complement(string read){
    string read_rc = read;
    reverse(read_rc.begin(), read_rc.end());
    for(int i =0; i<read_rc.length(); i++){
        char bg = 'N';
        switch (read_rc[i]) {
            case 'A':
                bg= 'T'; break;
            case 'T':
                bg= 'A'; break;
            case 'G':
                bg= 'C'; break;
            case 'C':
                bg= 'G'; break;
            default:
                break;
        }
        read_rc[i] = bg;
    }
    return read_rc;
}

/*
 contig<--> string
 */

string contig2string(vector<string> contig){
    string result;
    if(contig.size()>0){
        string end_str = contig.back();
        contig.pop_back();
        for(auto it= contig.begin(); it!= contig.end(); it++)
            result += (*it)[0];
        result += end_str;
    }
    return result;
}

/*
  读取fasta文件，存放到vector中
 */
vector<string> read_fasta(string filename){
    vector<string> reads;
    
    ifstream fin(filename);
    string line;
    
    while(getline(fin,line)){
        if(line[0]=='>')
            continue;
        reads.push_back(line);
    }
    fin.close();
    return reads;
}

/*
 将vector中的contigs输出到fasta文件
 */
void write_fasta(string filename, vector<string> reads){
    ofstream fout(filename);
    
    int num_id = 0;
    for(auto it = reads.begin(); it != reads.end(); it++){
        num_id++;
        fout<<">"<<"result"<<num_id<<endl;
        fout<<*it<<endl;
    }
    
}

#endif /* basic_functions_h */
