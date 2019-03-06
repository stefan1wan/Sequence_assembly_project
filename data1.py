# coding= utf-8

from basic_functions import *
from dbg import *
from Error_Handle import *

if __name__ == "__main__":

    sample = 2
    short_1 = "data"+str(sample) +"/short_1.fasta"
    short_2 = "data"+str(sample) +"/short_2.fasta"
    Long =    "data"+str(sample)+"/long.fasta"
    param_file ="data"+str(sample)+"/param.json"
    result_file ='result/'+ str(sample) +'_result'

    ###data1： k=30， thres = 15
    ###data2:  k=20  thres = 10

    # 读取数据、模式
    sequence_1 = readFasta(short_1)
    sequence_2 = readFasta(short_2)
    sequence_long = readFasta(Long)
    params = readParam(param_file)
    print(params)

    pair_distance = params["pair_distance"]

    k = 15
    
    sequence_1.extend(sequence_2)

    # 此处为single_dbg
    edges, nodes, path = build_DBG(k, sequence_1)
    #edges, nodes, path = build_comp_DBG(k, sequence_1)
    #edges, nodes, path = build_Paired_DBG(k, pair_distance, sequence_1, sequence_2)
    print(len(nodes))

    path, nodes = delete_bubbles(path, nodes, k, 500)
    # path, nodes = remove_tips(path, nodes, k)

    """
    阈值：计算平均覆盖度*rate
    含义：两点之间经过的次数若少于阈值，则假设不存在路径
    """
    # genome 长度: data1-3: 10,000 data4: 100,000

    # genome_length = 10000
    # rate = 0.333
    threshold = 2 #( len(sequence_1) * 2 * (params['short_read_length']-k) / genome_length * rate )

    ## 选取有效路径
    path = probability_path(path, threshold)

    ## 获取contigs
    contigs = find_long_Contigs(path,nodes)
    #contigs = findMNBP_comp(path,nodes)
    #contigs = findMNBP(path,nodes)
    print("contigs个数")
    print(len(contigs))

    print("超过一定长度的contigs")
    lens = []
    #contigs_r = []
    for x in contigs:
        if len(x) > 1000:#k+2: #2*k :
            lens.append(len(x))
            #contigs_r.append(x)
    print(lens)

    # contigs转化为输出格式
    out_result = []
    for x in contigs:#contigs:
        out_result.append(transSequence(x))

    writeFasta(out_result, result_file)
    