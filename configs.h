//
//  configs.h
//  DNA组装
//
//  Created by mov on 2018/6/22.
//  Copyright © 2018 万俊鹏. All rights reserved.
//

#ifndef configs_h
#define configs_h

#define DATA4

#ifdef DATA1
#define SHORT_1  "data1/short_1.fasta"
#define SHORT_2  "data1/short_2.fasta"
#define LONG   "data1/long.fasta"
#define PARAM  "data1/param.json"
#define RESULT_POS "result/result_1.fasta"
#define K 32
#define short_read_length  100
#define pair_distance 500
#define standard_deviation 0
#define short_read_eoor_rate 0
#define long_read_length 1000
#define long_read_error_rate 0.1
#define dup_time  34
#define COVERAGE_RATE  (dup_time * 5)
#define DIS_THRESHOLD 3
#endif

#ifdef DATA2
#define SHORT_1  "data2/short_1.fasta"
#define SHORT_2  "data2/short_2.fasta"
#define LONG   "data2/long.fasta"
#define PARAM  "data2/param.json"
#define RESULT_POS "result/result_2.fasta"
#define K 29
#define short_read_length  100
#define pair_distance 500
#define standard_deviation 5
#define short_read_eoor_rate 0.01
#define long_read_length 1000
#define long_read_error_rate 0.1
#define dup_time  20
#define COVERAGE_RATE  (dup_time * 5)
#define DIS_THRESHOLD 6
#endif

#ifdef DATA3
#define SHORT_1  "data3/short_1.fasta"
#define SHORT_2  "data3/short_2.fasta"
#define LONG   "data3/long.fasta"
#define PARAM  "data3/param.json"
#define RESULT_POS "result/result_3.fasta"
#define K 25
#define short_read_length  100
#define pair_distance 500
#define standard_deviation 10
#define short_read_eoor_rate 0.01
#define long_read_length 1000
#define long_read_error_rate 0.15
#define dup_time  10
#define COVERAGE_RATE  (dup_time * 5)
#define DIS_THRESHOLD 5
#endif

#ifdef DATA4
#define SHORT_1  "data4/short_1.fasta"
#define SHORT_2  "data4/short_2.fasta"
#define LONG   "data4/long.fasta"
#define PARAM  "data4/param.json"
#define RESULT_POS "result/result_4.fasta"
#define K 20
#define short_read_length  100
#define pair_distance 500
#define standard_deviation 10
#define short_read_eoor_rate 0.01
#define long_read_length 1000
#define long_read_error_rate 0.15
#define dup_time  10
#define COVERAGE_RATE  (dup_time * 2)
#define DIS_THRESHOLD 5
#endif

#endif /* configs_h */
