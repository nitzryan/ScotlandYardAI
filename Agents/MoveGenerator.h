#ifndef MOVEGENERATOR_H
#define MOVEGENERATOR_H

#include "../Shared.h"
#include "../SparseSquareMatrix.h"
#include <vector>

class MoveGenerator
{
public:
    MoveGenerator(std::vector<matrixVal> taxiMatrix,
                  std::vector<matrixVal> busMatrix,
                  std::vector<matrixVal> undergroundMatrix,
                  std::vector<matrixVal> ferryMatrix);

    std::vector<GameMove> GetPossibleMoves(unsigned char tile,
                                           unsigned char taxis,
                                           unsigned char buses,
                                           unsigned char undergrounds,
                                           unsigned char mysteries,
                                           unsigned char doubles,
                                           std::vector<unsigned char> avoidTiles) const;
private:
    SparseSquareMatrix taxiMatrix, busMatrix, undergroundMatrix, ferryMatrix;
};

#endif // MOVEGENERATOR_H
