#include <fstream>
#include <iostream>
#include <vector>
#include "apis_c.h"

#define Row 3000
#define Col 1000

// 300*100 和 100*300 相乘
int idX, idY;
using namespace std;
int main(int argc, char **argv) {
    idX = atoi(argv[1]);
    idY = atoi(argv[2]);
    vector <int64_t*> A,B;
    for(int i=0;i<3;i++){
        int64_t *a = (int64_t *)malloc(sizeof(int64_t) * Row/3 * Col);
        for (int i = 0; i < Row/3 * Col; i++) {
            a[i] = rand() % 51;
        }
        A.push_back(a);
    }
    for(int i=0;i<3;i++){
        int64_t *b = (int64_t *)malloc(sizeof(int64_t) * Row/3 * Col);
        for (int i = 0; i < Row/3 * Col; i++) {
            b[i] = rand() % 51;
        }
        B.push_back(b);
    }
    vector <int64_t*> Result;
    for(int i=0;i<9;i++){
        int64_t *C = (int64_t *)malloc(sizeof(int64_t) * Row * Col/3);
        Result.push_back(C);
    }

    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            InterChiplet::sendMessage(i, j, idX, idY, A[i], Row/3 * Col * sizeof(int64_t));
            InterChiplet::sendMessage(i, j, idX, idY, B[j], Row * Col/3 * sizeof(int64_t));
        }
    }

    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            InterChiplet::receiveMessage(idX, idY, i, j, Result[i+j*3], Row * Col/3 * sizeof(int64_t));
        }
    }
    // InterChiplet::sendMessage(0, 1, idX, idY, A, Row * Col * sizeof(int64_t));
    // InterChiplet::sendMessage(1, 0, idX, idY, A, Row * Col * sizeof(int64_t));
    // InterChiplet::sendMessage(1, 1, idX, idY, A, Row * Col * sizeof(int64_t));

    // InterChiplet::sendMessage(0, 1, idX, idY, B, Row * Col * sizeof(int64_t));
    // InterChiplet::sendMessage(1, 0, idX, idY, B, Row * Col * sizeof(int64_t));
    // InterChiplet::sendMessage(1, 1, idX, idY, B, Row * Col * sizeof(int64_t));

    // InterChiplet::receiveMessage(idX, idY, 0, 1, C1, Row * Col * sizeof(int64_t));
    // InterChiplet::receiveMessage(idX, idY, 1, 0, C2, Row * Col * sizeof(int64_t));
    // InterChiplet::receiveMessage(idX, idY, 1, 1, C3, Row * Col * sizeof(int64_t));

    // for (int i = 0; i < 100; i++) {
    //     C1[i] += C2[i];
    //     C1[i] += C3[i];
    // }
}
