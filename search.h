#ifndef SEARCH_H
#define SEARCH_H

#include <unordered_map>

#include "defs.h"
#include "board.h"
#include "eval.h"

class Search {
	public:
		Search();
		move_t think(Board board, int depth);

	private:
		search_t alphaBetaRoot(Board& board, int depth);
		int alphaBeta(Board& board, int alpha, int beta, int depth);
		int quiesce(Board& board, int alpha, int beta);
		void tt_save(Board& board, move_t move, int depth, int score, int node_type);
		
		Eval eval;
		std::unordered_map<int64_t, tt_entry> tt; // transposition table
};

#endif