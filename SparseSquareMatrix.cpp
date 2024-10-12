#include "SparseSquareMatrix.h"
#include <algorithm>

SparseSquareMatrix::SparseSquareMatrix(std::vector<matrixVal> mVals)
{
    vals = mVals;
    std::sort(vals.begin(), vals.end(), [](matrixVal& a, matrixVal& b) {
        return a.row < b.row || (a.row == b.row && a.col < b.col);
    });
}

bool SparseSquareMatrix::IsTrue(unsigned char row, unsigned char col) const
{
    return IsTrue(row, col, 0, vals.size() - 1);
}

std::vector<unsigned char> SparseSquareMatrix::GetRow(unsigned char row) const
{
    int idx = GetRowIdx(row, 0, vals.size() - 1);
    if (idx < 0) {
        return {};
    }

    // Go from midpoint, go backwards until not same row
    int idxStart = idx;
    while(idxStart >= 0) {
        const matrixVal& val = vals[idxStart];
        if (val.row == row) {
            idxStart--;
        } else {
            break;
        }
    }
    while (idx < vals.size()) {
        const matrixVal& val = vals[idx];
        if (val.row == row) {
            idx++;
        } else {
            break;
        }
    }

    // Will result in one extra increment, so undo
    idxStart++;

    std::vector<unsigned char> result;
    for (size_t i = idxStart; i < idx; i++) {
        result.push_back(vals[i].col);
    }
    return result;
}

bool SparseSquareMatrix::IsTrue(unsigned char row, unsigned char col, std::size_t start, std::size_t end) const
{
    if (start == end) {
        const matrixVal& val = vals[start];
        return val.row == row && val.col == col;
    } else if ((start + 1) == end) {
        return IsTrue(row, col, start, start) || IsTrue(row, col, end, end);
    }

    std::size_t midpoint = (end + start) / 2;
    const matrixVal& midVal = vals[midpoint];
    if (midVal.row > row || (midVal.row == row && midVal.col > col)) {
        return IsTrue(row, col, start, midpoint);
    } else {
        return IsTrue(row, col, midpoint, end);
    }
}

int SparseSquareMatrix::GetRowIdx(unsigned char row, std::size_t start, std::size_t end) const
{
    if (start == end) {
        const matrixVal& val = vals[start];
        if (val.row == row)
            return start;
        return -1;
    } else if ((start + 1) == end) {
        int v1 = GetRowIdx(row, start, start);
        int v2 = GetRowIdx(row, end, end);
        if (v1 > v2)
            return v1;
        return v2;
    }

    std::size_t midpoint = (end + start) / 2;
    const matrixVal& midVal = vals[midpoint];
    if (midVal.row > row) {
        return GetRowIdx(row, start, midpoint);
    } else {
        return GetRowIdx(row, midpoint, end);
    }
}
