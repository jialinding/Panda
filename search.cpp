#include <cstdlib> // rand

#include "search.h"

Search::Search() {
	eval = Eval();
}

move_t Search::think(Board board, int depth) {
	// move_t bestMove;
	std::vector<move_t> bestMoves;
	int max = -100000;
	int score;
	for (move_t& move : board.generateMoves()) {
		if (board.move(move)) {
			score = -negaMax(board, depth-1);
			board.undoMove();
			if (score > max) {
				max = score;
				// bestMove = move;
				bestMoves.clear();
				bestMoves.push_back(move);
			} else if (score == max) {
				bestMoves.push_back(move);
			}
		}
	}

	// return bestMove;
	int bestMoveIndex = rand() % bestMoves.size();
	return bestMoves[bestMoveIndex];
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
