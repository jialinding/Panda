#ifndef EVAL_H
#define EVAL_H

#include "defs.h"
#include "board.h"

class Eval {
	public:
		Eval();
		int evaluate(Board board);
};

#endif