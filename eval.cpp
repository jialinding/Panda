#include "eval.h"

Eval::Eval() {}

int Eval::evaluate(const Board& board) {
	int score = 0;
	for (int sq = 0; sq < 64; ++sq) {
		// add value to own pieces
		if (board.color[sq] == board.side) {
			score += piece_values[board.pieces[sq]];
			switch (board.pieces[sq]) {
				case 0: score += pawn_pcsq[sq];
				case 1: score += knight_pcsq[sq];
				case 2: score += bishop_pcsq[sq];
				case 5: score += king_pcsq[sq];
				default: continue;
			}
		}

		// subtract value of opponent pieces
		if (board.color[sq] == board.xside) {
			score -= piece_values[board.pieces[sq]];
			switch (board.pieces[sq]) {
				case 0: score -= pawn_pcsq[63-sq];
				case 1: score -= knight_pcsq[63-sq];
				case 2: score -= bishop_pcsq[63-sq];
				case 5: score -= king_pcsq[63-sq];
				default: continue;
			}
		}
	}

	return score;
}