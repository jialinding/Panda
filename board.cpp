#include <algorithm>
#include <iostream>

#include "board.h"
#include "defs.h"
#include "utils.h"

Board::Board() : side(WHITE) {
	color = init_color;
	pieces = init_pieces;
}

void Board::move(move_t move) {
	color[move.from] = EMPTY;
	color[move.to] = side;
	pieces[move.to] = pieces[move.from];
	pieces[move.from] = _;
	Board::rotateBoard();
	side = side == WHITE ? BLACK : WHITE;
}

std::vector<move_t> Board::generateMoves() {
	std::vector<move_t> moves;
	for (int sq = 0; sq < 64; sq++) {
		if (color[sq] != side) continue;

		int piece = pieces[sq];
	}
	return moves;
}

void Board::display() {
	for (int sq = 0; sq < 64; sq++) {
		if (color[sq] == EMPTY) {
			std::cout << "*";
		} else {
			std::cout << displayPiece(pieces[sq], color[sq]);
		}

		if (sq % 8 == 7) {
			std::cout << "\n";
		}
	}
}

void Board::rotateBoard() {
	std::reverse(color.begin(), color.end());
	std::reverse(pieces.begin(), pieces.end());
}