#include <fstream>
#include <iostream>
#include <vector>
#include "apis_c.h"

int Row = 100;
int Col = 100;

// 300*100 和 100*300 相乘
int idX, idY;
using namespace std;
int main(int argc, char **argv) {
    idX = atoi(argv[1]);
    idY = atoi(argv[2]);
    vector<int64_t*> A;
    for(int i = 0; i < 3; i++) {
        int64_t *a = new int64_t[Row/3 * Col];
        for (int i = 0; i < Row/3 * Col; i++) {
            a[i] = rand() % 51;
        }
        A.push_back(a);
    }
    int64_t *a = new int64_t[Row % (Row/3) * Col];
    for (int i = 0; i < Row % (Row/3) * Col; i++) {
        a[i] = rand() % 51;
    }
    A.push_back(a);

    int64_t *B = new int64_t[Col * Row];
    for (int i = 0; i < Col * Row; i++) {
        B[i] = rand() % 51;
    }
    vector<int64_t*> Result;
    for(int i = 0; i < 3; i++) {
        int64_t *C = new int64_t[Row * Col/3];
        Result.push_back(C);
    }
    int64_t *C = new int64_t[Row * Col % (Row/3)];
    Result.push_back(C);
    for(int i = 0; i < 3; i++) {
        InterChiplet::sendMessage(0, i, idX, idY, A[i], Row/3 * Col * sizeof(int64_t));
        InterChiplet::sendMessage(0, i, idX, idY, B, Row * Col * sizeof(int64_t));
    }
    InterChiplet::sendMessage(0, 3, idX, idY, A[3], Row % (Row/3) * Col * sizeof(int64_t));
    InterChiplet::sendMessage(0, 3, idX, idY, B, Row * Col * sizeof(int64_t));

    for(int i = 0; i < 3; i++) {
        InterChiplet::receiveMessage(idX, idY, 0, i, Result[i], Row * (Col/3) * sizeof(int64_t));
    }

    InterChiplet::receiveMessage(idX, idY, 0, 3, Result[3], Row * (Col % (Row/3)) * sizeof(int64_t));
    for(int i = 0; i < Row; i++) {
        for(int j = 0; j < Col; j++) {
            cout << Result[i/33][j+i%33*100] << " ";
        }
        cout << endl;
    }
    for(int i = 0; i < 4; i++) {
        delete[] A[i];
        delete[] Result[i];
    }
    delete[] B;
}
