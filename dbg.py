# coding= utf-8

from itertools import permutations
from basic_functions import reverseComplement

def build_DBG(k, reads):
    edges = []
    nodes = set()
    path = {}
    """
    先将reads_1 和reads_2按k打碎, 前后组成一对
    """
    reads_re_co = []
    for x in reads:
        reads_re_co.append( reverseComplement(x) )

    reads.extend(reads_re_co)

    for x in reads:
        for i in range(len(x) - k +1):
            nodes.add(x[i:i+k])
            if i != len(x) - k:
                if x[i:i+k] in path.keys():
                    if x[i+1:i+k+1] in path[x[i:i+k]].keys():
                        path[x[i:i+k]][x[i+1:i+k+1]] += 1
                    else:
                        path[x[i:i+k]][x[i+1:i+k+1]] =  1

                else:
                    path[x[i:i+k]] = { x[i+1:i+k+1] :1, }

            edges.append((x[i:i+k], x[i+1:i+k+1] ))
    return edges, nodes, path


# 只有经过的数目大于一定阈值，才计算在内
def probability_path(path, threshold):
    """
    小于阈值的边不计算在内
    """
    probability_path = {}
    for x in path.keys():
        #print(path[x])
        crossroad = []
        for y in path[x].keys():
            # if path[x][y]>15:
            #     print(path[x][y])
            if path[x][y] > threshold:
                crossroad.append(y)
        if len(crossroad) > 0 :
            probability_path[x] = crossroad
    return probability_path


def build_dgree(path, nodes):
    in_ = {}
    out_ = {}
    for x in path.keys():
        out_[x] = len(path[x])
        for y in path[x]:
            if y not in in_.keys():
                in_[y] = 1
            else:
                in_[y] += 1
    
    for x in nodes:
        if x not in in_.keys():
            in_[x] = 0

        if x not in out_.keys():
            out_[x] = 0
    return in_, out_


"""算法描述：
    MaximalNonBranchingPaths(Graph)
        Paths ← empty list
        for each node v in Graph
            if v is not a 1-in-1-out node
                if out(v) > 0
                    for each outgoing edge (v, w) from v
                        NonBranchingPath ← the path consisting of the single edge (v, w)
                        while w is a 1-in-1-out node
                            extend NonBranchingPath by the outgoing edge (w, u) from w 
                            w ← u
                        add NonBranchingPath to the set Paths
        for each isolated cycle Cycle in Graph
            add Cycle to Paths
        return Paths
"""

def findMNBP(path,nodes):
    ins, outs = build_dgree(path, nodes)
    contigs = []
    marked = set() # 对遍历过顶点作标记，方便下一步找到单环
    for x in nodes:
        if ins[x] != 1 or outs[x] != 1:
            if outs[x] > 0:
                marked.add(x)
                for y in path[x]:
                    contig = [x, ]
                    y_tem = y
                    while ins[y_tem] == 1 and outs[y_tem] ==1:
                        marked.add(y_tem)
                        contig.append(y_tem)
                        y_tem = path[y_tem][0]

                    marked.add(y_tem)
                    contig.append(y_tem)
                    if contig:
                        contigs.append(contig)

    # 添加其他路径
    nodes_others = nodes - marked
    for x in nodes_others:
        if x in marked or x not in path.keys():
            continue
        ring = [x,]
        x_find = path[x][0]
        while ins[x_find] == 1 and outs[x_find] ==1 and x_find not in marked and x_find != x:
            ring.append(x_find)
            x_find = path[x_find][0]
            marked.add(x_find)

        contigs.append(ring)
    return contigs

def find_a_start(path, marked, nodes, ins, outs):
    candidates = nodes - marked
    if len(candidates)==0:
        return 'stop'
    for x in candidates:
        if ins[x] ==0 and outs[x]> 0:
            return x

    re = candidates.pop()
    candidates.add(re)
    return re



def find_long_Contigs(path,nodes):
    # 使用深搜
    ins, outs = build_dgree(path, nodes)
    contigs = []
    marked = set() # 对遍历过顶点作标记，方便下一步找到单环
    for x in nodes:
        start = find_a_start(path, marked, nodes, ins, outs)
        if start == 'stop':
            break

        x = start
        if outs[x] <0:
            continue
        #if ins[x] != 1 or outs[x] != 1:
        marked.add(x)
        if x not in path.keys():
            continue

        for y in path[x]:
            contig = [x, ]
            y_tem = y
            while outs[y_tem] >0 and y_tem not in marked:
                marked.add(y_tem)
                contig.append(y_tem)
                y_tem = path[y_tem][0]

            # while ins[y_tem] == 1 and outs[y_tem] ==1:
            #     marked.add(y_tem)
            #     contig.append(y_tem)
            #     y_tem = path[y_tem][0]
            marked.add(y_tem)
            contig.append(y_tem)
            if contig:
                contigs.append(contig)
                #print(contig)
        print(len(contigs))

    return contigs

if __name__ == "__main__":
    pass
