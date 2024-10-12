#ifndef SPARSESQUAREMATRIX_H
#define SPARSESQUAREMATRIX_H

#include <vector>
#include "Shared.h"

class SparseSquareMatrix
{
public:
    SparseSquareMatrix(std::vector<matrixVal> mVals);
    bool IsTrue(unsigned char row, unsigned char col) const;
    std::vector<unsigned char> GetRow(unsigned char row) const;
private:
    std::vector<matrixVal> vals;

    bool IsTrue(unsigned char row, unsigned char col, std::size_t start, std::size_t end) const;
    int GetRowIdx(unsigned char row, std::size_t start, std::size_t end) const;
};

#endif // SPARSESQUAREMATRIX_H
