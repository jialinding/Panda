#include <iostream> //debug
#include "search.h"

Search::Search() {
	eval = Eval();
}

move_t Search::think(Board board, int depth) {
	move_t bestMove;
	int max = -100000;
	int score;
	for (move_t& move : board.generateMoves()) {
		if (board.move(move)) {
			std::cout << "think " << move.from << " " << move.to << std::endl;
			score = -negaMax(board, depth-1);
			std::cout << "score " << score << std::endl;
			board.undoMove();
			if (score > max) {
				max = score;
				bestMove = move;
			}
		}
	}

	return bestMove;
}

int Search::negaMax(Board& board, int depth) {
	if (depth == 0) return eval.evaluate(board);

	int max = -100000; // some low number
	int score;
	bool legal_moves = false;
	
	for (move_t& move : board.generateMoves()) {
		if (board.move(move)) {
			legal_moves = true;
			score = -negaMax(board, depth-1);
			board.undoMove();
			if (score > max) max = score;
		}
	}

	if (!legal_moves) {
		if (board.isCheck(board.side)) return -99999; // checkmate
		else return 0; // stalemate
	}

	return max;
}
