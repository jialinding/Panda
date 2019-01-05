#include <cstdlib> // rand
#include <iostream>

#include "search.h"
#include "utils.h"

Search::Search() {
	eval = Eval();
}

/* Wrapper function that uses iterative deepening to find the best move */
move_t Search::think(Board board, int depth) {
	search_t bestMove;
	int score;
	std::cout << "depth 0 " << eval.evaluate(board) << std::endl;
	for (int d = 1; d <= depth; ++d) {
		std::cout << "depth " << d << std::endl;
		bestMove = alphaBetaRoot(board, d);
		std::cout << "	bestmove " << parseMove(bestMove.move, board.side) << " " <<
			bestMove.score << std::endl;
	}
	return bestMove.move;
}

/* Root search function that returns a move instead of a score */
search_t Search::alphaBetaRoot(Board& board, int depth) {
	int max = -100000; // keeps maximum score
	int score;
	search_t bestMove; // to be returned
	// move_t bestMove;
	std::vector<move_t> bestMoves;

	// check transposition table
	if (tt.find(board.hash()) != tt.end()) {
		tt_entry entry = tt[board.hash()];
		if (entry.node_type == EXACT_NODE) {
			if (entry.depth >= depth) {
				bestMove = {entry.best_move, entry.score};
				return bestMove;
			}
			
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
			std::cout << "	" << parseMove(move, board.side) << " " << score << std::endl;
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
	bestMove = {bestMoves[bestMoveIndex], max};
	return bestMove;
}

/* Search function using alpha-beta pruning in a negamax framework */
int Search::alphaBeta(Board& board, int alpha, int beta, int depth) {
	if (depth == 0) {
		if (!board.isCheck(board.side)) return eval.evaluate(board); // quiesce(board, alpha, beta);
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
