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
		int negaMax(Board& board, int depth);
		Eval eval;
};

#endif