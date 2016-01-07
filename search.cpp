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
			score = -alphaBeta(board, -100000, 100000, depth-1);
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

int Search::alphaBeta(Board& board, int alpha, int beta, int depth) {
	if (depth == 0) return quiesce(board, alpha, beta);

	int score;
	bool legal_moves = false;	
	for (move_t& move : board.generateMoves()) {
		if (board.move(move)) {
			legal_moves = true;
			score = -alphaBeta(board, -beta, -alpha, depth-1);
			board.undoMove();
			if (score >= beta) return beta;
			if (score > alpha) alpha = score;
		}
	}

	if (!legal_moves) {
		if (board.isCheck(board.side)) return -99999; // checkmate
		else return 0; // stalemate
	}

	return alpha;
}

int Search::quiesce(Board& board, int alpha, int beta) {
	int stand_pat = eval.evaluate(board);
	if (stand_pat >= beta) return beta;
	if (alpha < stand_pat) alpha = stand_pat;

	int score;
	for (move_t& move : board.generateCaptures()) {
		if (board.move(move)) {
			score = -quiesce(board, -beta, -alpha);
			board.undoMove();
			if (score >= beta) return beta;
			if (score > alpha) alpha = score;
		}
	}

	return alpha;
}
