#ifndef UTILS_H
#define UTILS_H

#include <string>

#include "defs.h"

move_t parseInput(std::string input);
std::string displayPiece(Piece piece, int color);

#endif