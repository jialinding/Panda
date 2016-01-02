#include "eval.h"

std::vector<int> piece_values = {100, 300, 300, 500, 900, 0, 0};

Eval::Eval() {}

int Eval::evaluate(Board board) {
	int score = 0;
	for (int sq = 0; sq < 64; ++sq) {
		// add value to own pieces
		if (board.color[sq] == board.side) {
			score += piece_values[board.pieces[sq]];
		}

		// subtract value of opponent pieces
		if (board.color[sq] == board.xside) {
			score -= piece_values[board.pieces[sq]];
		}
	}

	return score;
}