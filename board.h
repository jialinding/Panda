#ifndef BOARD_H
#define BOARD_H

#include <array>
#include <stack>

#include "defs.h"

class Board {
	public:
		Board();
		int64_t hash();
		bool move(move_t move);
		void undoMove();
		bool isAttacked(int square, int attacking_side);
		bool isCheck(int checked_side);
		std::vector<move_t> generateMoves();
		std::vector<move_t> generateCaptures();
		void display();

		int side;
		int xside;
		std::array<int, 64> color;
		std::array<Piece, 64> pieces;

	private:
		int side_castle; // 1: kingside, 2: queenside
		int xside_castle;
		int ep;
		std::stack<hist_t> move_history;
		
		// hash constants
		int64_t hash_board[2][6][64];
		int64_t hash_side;
		int64_t hash_ep[64];
		int64_t hash_castle[4][4];

		void rotateBoard();
};

#endif