#ifndef UTILS_H
#define UTILS_H

#include <string>

#include "defs.h"
#include "board.h"

int parseInput(std::string input, int side, const std::vector<move_t>& moves);
std::string parseMove(move_t move, int side);
std::string displayPiece(Piece piece, int color);
bool printResult(Board& board);

#endif