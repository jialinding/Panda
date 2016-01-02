#ifndef UTILS_H
#define UTILS_H

#include <string>

#include "defs.h"
#include "board.h"

int parseInput(std::string input, const std::vector<move_t>& moves);
std::string displayPiece(Piece piece, int color);
bool printResult(Board& board);

#endif