#include <cstdlib> // rand

#include "search.h"

Search::Search() {
	eval = Eval();
}

/* Wrapper function that uses iterative deepening to find the best move */
move_t Search::think(Board board, int depth) {
	move_t bestMove;
	for (int d = 1; d <= depth; ++d) {
		bestMove = alphaBetaRoot(board, d);
	}
	return bestMove;
}

/* Root search function that returns a move instead of a score */
move_t Search::alphaBetaRoot(Board& board, int depth) {
	int max = -100000; // keeps maximum score
	int score;
	// move_t bestMove;
	std::vector<move_t> bestMoves;

	// check transposition table
	if (tt.find(board.hash()) != tt.end()) {
		tt_entry entry = tt[board.hash()];
		if (entry.node_type == EXACT_NODE) {
			if (entry.depth >= depth) return entry.best_move;
			
			// search the move from the transposition table first
			if (board.move(entry.best_move)) {
				score = -alphaBeta(board, -100000, 100000, depth-1);
				board.undoMove();
				if (score > max) {
					max = score;
					bestMoves.push_back(entry.best_move);
				}
			}
		}
	}
	
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
	tt_save(board, bestMoves[bestMoveIndex], depth, max, EXACT_NODE);
	return bestMoves[bestMoveIndex];
}

/* Search function using alpha-beta pruning in a negamax framework */
int Search::alphaBeta(Board& board, int alpha, int beta, int depth) {
	if (depth == 0) {
		if (!board.isCheck(board.side)) return quiesce(board, alpha, beta);
		else depth = 1;
	}

	int score;
	bool legal_moves = false;

	// check transposition table
	if (tt.find(board.hash()) != tt.end()) {
		tt_entry entry = tt[board.hash()];
		if (entry.node_type == EXACT_NODE) {
			if (entry.depth >= depth) return entry.score;
			
			// search the move from the transposition table first
			if (board.move(entry.best_move)) {
				legal_moves = true;
				score = -alphaBeta(board, -beta, -alpha, depth-1);
				board.undoMove();
				if (score >= beta) return beta;
				if (score > alpha) {
					alpha = score;
					tt_save(board, entry.best_move, depth, alpha, EXACT_NODE);
				}
			}
		}
	}

	// search all other moves (unfortunately, this also includes the move from the
	// transposition table, if one existed)
	for (move_t& move : board.generateMoves()) {
		if (board.move(move)) {
			legal_moves = true;
			score = -alphaBeta(board, -beta, -alpha, depth-1);
			board.undoMove();
			if (score >= beta) return beta;
			if (score > alpha) {
				alpha = score;
				tt_save(board, move, depth, alpha, EXACT_NODE);
			}
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

void Search::tt_save(Board& board, move_t move, int depth, int score, int node_type) {
	int64_t hash = board.hash();
	tt_entry entry = {move, depth, score, node_type};
	tt[hash] = entry;
}
