# include <stdint.h>
#include <stdio.h>
#include <sys/time.h>
#include <math.h>

#include <fstream>
#include <iostream>
#include <string>

#include "apis_c.h"
/**
 * 本示例程序为：通过4个GPU chiplet
 * 计算随机数矩阵A（400 * 100）与随机数矩阵B（100 * 400）相乘结果。
 * 由矩阵乘法原理可知，我们可将计算任务划分为4个100*100的矩阵相乘，并将结果相加。
 */

int Row_A = 33;
int Col_A = 100;
int Row_B = 100;
int Col_B = 100;

/**
 * 矩阵乘法的核心函数，由每个线程都会运行一次本函数，
 * 根据线程编号不同计算出位于结果矩阵不同位置的数据。
 */

void matrix_mul_cpu(const int64_t* M, const int64_t* N, int64_t* P, int height_M, int width_M, int width_N) {
    for (int i = 0; i < height_M; ++i) {
        for(int j=0;j<width_N;j++){
            P[i * width_N + j] = 0;
            for (int k = 0; k < width_M; ++k) {
                P[i * width_N + j] += M[i * width_M + k] * N[k * width_N + j];
            }
        } 
    }
}

int main(int argc, char** argv) {
    // 读取本进程所代表的chiplet编号
    int idX = atoi(argv[1]);
    int idY = atoi(argv[2]);
    if(idY == 3){
        Row_A = 1;
    }
    int64_t* dataA = new int64_t[Row_A * Col_A];
    int64_t* dataB = new int64_t[Row_B * Col_B];
    int64_t* dataC = new int64_t[Row_A * Col_B];

    InterChiplet::receiveMessage(idX, idY, 3, 3, dataA, sizeof(int64_t) * Row_A * Col_A);
    InterChiplet::receiveMessage(idX, idY, 3, 3, dataB, sizeof(int64_t) * Row_B * Col_B);

    // calculate
    std::cout<<"--------------------------------------开始计算--------------------------------------"<<std::endl;
    matrix_mul_cpu(dataA, dataB, dataC, Row_A,Col_A,Col_B);
    std::cout<<"--------------------------------------计算完成--------------------------------------"<<std::endl;
    InterChiplet::sendMessage(3, 3, idX, idY, dataC, Row_A * Col_B * sizeof(int64_t));

    delete[] dataA;
    delete[] dataB;
    delete[] dataC;

    return 0;
}
