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

#define Row 1000
#define Col 1000

/**
 * 矩阵乘法的核心函数，由每个线程都会运行一次本函数，
 * 根据线程编号不同计算出位于结果矩阵不同位置的数据。
 */

void matrix_mul_cpu(const int64_t* M, const int64_t* N, int64_t* P, int width) {
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < width; ++j) {
            int64_t sum = 0;
            for (int k = 0; k < width; ++k) {
                sum += M[j * width + k] * N[k * width + i];
            }
            P[j * width + i] = sum;
        }
    }
}

int main(int argc, char** argv) {
    // 读取本进程所代表的chiplet编号
    int idX = atoi(argv[1]);
    int idY = atoi(argv[2]);

    int64_t* dataA = new int64_t[Row * Col];
    int64_t* dataB = new int64_t[Row * Col];
    int64_t* dataC = new int64_t[Row * Col];

    InterChiplet::receiveMessage(idX, idY, 3, 3, dataA, sizeof(int64_t) * Row * Col);
    InterChiplet::receiveMessage(idX, idY, 3, 3, dataB, sizeof(int64_t) * Row * Col);

    // calculate
    matrix_mul_cpu(dataA, dataB, dataC, Col);

    InterChiplet::sendMessage(3, 3, idX, idY, dataC, Row * Col * sizeof(int64_t));

    delete[] dataA;
    delete[] dataB;
    delete[] dataC;

    return 0;
}
