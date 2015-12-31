#ifndef BOARD_H
#define BOARD_H

#include <array>
#include <stack>

#include "defs.h"

class Board {
	public:
		Board();
		bool move(move_t move);
		void undoMove();
		bool isAttacked(int square);
		bool isCheck();
		std::vector<move_t> generateMoves();
		void display();

		int side;
		int xside;

	private:
		int side_castle; // 1: kingside, 2: queenside
		int xside_castle;
		int ep;
		std::array<int, 64> color;
		std::array<Piece, 64> pieces;
		std::stack<hist_t> move_history;

		void rotateBoard();
};

#endif