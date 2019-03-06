# coding = utf-8 

import json

def readFasta(filename):
    sequence=[]
    file = open(filename, "r");
    while True:
    	file.readline()
    	line=file.readline().rstrip()
    	if len(line) == 0:
    		break

    	sequence.append(line)
    return sequence

def readParam(filename):
    load_dict = {}
    with open(filename, 'r') as load_f:
        load_dict = json.load(load_f)
    return load_dict

def writeFasta(sequence, filename):
    file = open(filename, "w")
    for i in range(len(sequence)):
        file.write(">Result %s\n"%(i+1))
        file.write(sequence[i])
        file.write('\n')
    file.close()



def reverseComplement(s):
    """得到反向互补链
    """
    complement= {'A':'T', 'C':'G', 'G':'C', 'T':'A', 'N':'N'}
    t = ''
    for base in s:
        # 后加t, 得到反向互补链
        t = complement[base]+t
    return t

def Complement(s):
    """得到互补链
    """
    complement= {'A':'T', 'C':'G', 'G':'C', 'T':'A', 'N':'N'}
    t = ''
    for base in s:
        # 前加t, 得到互补链
        t = t + complement[base]
    return t


"""
将序列转化成字符串
"""

def transSequence(sequence):
    result = ''
    for i in range(len(sequence)):
        if i == 0:
            result += sequence[i]
        else:
            result += sequence[i][-1]

    return result


def transSequence_comp(sequence):
    
    ori_sequence = sequence
    sequence = [ori_sequence[0], ]
    for i in range(1, len(ori_sequence)):
        if ori_sequence[i][0] == sequence[i-1][0]:
            sequence.append(ori_sequence[i])
        else:
            sequence.append((ori_sequence[i][1], ori_sequence[i][0]))

    result = ''
    for i in range(len(sequence)):
        if i == 0:
            result += sequence[i][0]
        else:
            result += sequence[i][0][-1]
    return result

def transSequence_paired(sequence, k, d):
    result_1 = ''
    for i in range(len(sequence)):
        if i == 0:
            result_1 += sequence[i][0]
        else:
            result_1 += sequence[i][0][-1]

    result_2 = ''
    for i in range(len(sequence)):
        if i == 0:
            result_2 += sequence[i][1]
        else:
            result_2 += sequence[i][1][-1]

    result = result_1[:k+d] + result_2
    return result

if __name__ == "__main__":
    readParam("data1/param.json")
    sequence=readFasta("data1/short_1.fasta")
    # for x in sequence:
    # 	print(x)
    # 	print("---")