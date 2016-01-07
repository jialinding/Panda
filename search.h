#ifndef SEARCH_H
#define SEARCH_H

#include "defs.h"
#include "board.h"
#include "eval.h"

class Search {
	public:
		Search();
		move_t think(Board board, int depth);

	private:
		int alphaBeta(Board& board, int alpha, int beta, int depth);
		int quiesce(Board& board, int alpha, int beta);
		Eval eval;
};

#endif