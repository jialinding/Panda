#include "eval.h"

Eval::Eval() {}

int Eval::evaluate(const Board& board) {
	int score = 0;
	for (int sq = 0; sq < 64; ++sq) {
		// add value to own pieces
		if (board.color[sq] == board.side) {
			score += piece_values[board.pieces[sq]];
			switch (board.pieces[sq]) {
				case P: score += pawn_pcsq[sq];
				case N: score += knight_pcsq[sq];
				case B: score += bishop_pcsq[sq];
				case K: score += king_pcsq[sq];
				default: continue;
			}
		}

		// subtract value of opponent pieces
		if (board.color[sq] == board.xside) {
			score -= piece_values[board.pieces[sq]];
			switch (board.pieces[sq]) {
				case P: score -= pawn_pcsq[flip[sq]];
				case N: score -= knight_pcsq[flip[sq]];
				case B: score -= bishop_pcsq[flip[sq]];
				case K: score -= king_pcsq[flip[sq]];
				default: continue;
			}
		}
	}

	return score;
}