#ifndef BOARD_H
#define BOARD_H

#include <array>

#include "defs.h"

class Board {
	public:
		Board();
		void move(move_t move);
		std::vector<move_t> generateMoves();
		void display();

	private:
		int side;
		std::array<int, 64> color;
		std::array<Piece, 64> pieces;

		void rotateBoard();
};

#endif