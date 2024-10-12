#include "MoveGenerator.h"
#include <algorithm>

MoveGenerator::MoveGenerator(std::vector<matrixVal> taxiMatrix, std::vector<matrixVal> busMatrix, std::vector<matrixVal> undergroundMatrix, std::vector<matrixVal> ferryMatrix)
    : taxiMatrix(taxiMatrix), busMatrix(busMatrix), undergroundMatrix(undergroundMatrix), ferryMatrix(ferryMatrix)
{

}

std::vector<GameMove> MoveGenerator::GetPossibleMoves(unsigned char tile, unsigned char taxis, unsigned char buses, unsigned char undergrounds, unsigned char mysteries, unsigned char doubles, std::vector<unsigned char> avoidTiles) const
{
    auto taxiTiles = taxiMatrix.GetRow(tile);
    auto busTiles = busMatrix.GetRow(tile);
    auto undergroundTiles = undergroundMatrix.GetRow(tile);
    auto ferryTiles = ferryMatrix.GetRow(tile);

    std::vector<GameMove> moves;
    for (auto& tile : taxiTiles) {
        if (std::find(avoidTiles.begin(), avoidTiles.end(), tile) != avoidTiles.end())
            continue;
        if (taxis > 0) {
            moves.push_back({(unsigned char)GameMovesType::TAXI, tile});
        }
        if (mysteries > 0) {
            moves.push_back({(unsigned char)GameMovesType::MYSTERY, tile});
        }
    }

    for (auto& tile : busTiles) {
        if (std::find(avoidTiles.begin(), avoidTiles.end(), tile) != avoidTiles.end())
            continue;
        if (buses > 0) {
            moves.push_back({(unsigned char)GameMovesType::BUS, tile});
        }
        if (mysteries > 0) {
            moves.push_back({(unsigned char)GameMovesType::MYSTERY, tile});
        }
    }

    for (auto& tile : undergroundTiles) {
        if (std::find(avoidTiles.begin(), avoidTiles.end(), tile) != avoidTiles.end())
            continue;
        if (undergrounds > 0) {
            moves.push_back({(unsigned char)GameMovesType::UNDERGROUND, tile});
        }
        if (mysteries > 0) {
            moves.push_back({(unsigned char)GameMovesType::MYSTERY, tile});
        }
    }

    for (auto& tile : ferryTiles) {
        if (std::find(avoidTiles.begin(), avoidTiles.end(), tile) != avoidTiles.end())
            continue;
        if (mysteries > 0) {
            moves.push_back({(unsigned char)GameMovesType::MYSTERY, tile});
        }
    }

    if (doubles > 0) { // Copy entire list with a double added
        std::size_t len = moves.size();
        for (std::size_t i = 0; i < len; i++) {
            GameMove gm = moves[i];
            gm.type += (unsigned char)GameMovesType::DOUBLE;
            moves.push_back(gm);
        }
    }

    return moves;
}
