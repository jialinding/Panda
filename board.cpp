#include <algorithm>
#include <iostream>

#include "board.h"
#include "defs.h"
#include "utils.h"

Board::Board() : side(WHITE), xside(BLACK), side_castle(3), xside_castle(3), ep(-1) {
	color = init_color;
	pieces = init_pieces;
}

bool Board::move(move_t move) {
	// add to move history stack
	Piece captured_piece;
	if (move.bits & 1) {
		captured_piece = pieces[move.to];
	} else {
		captured_piece = _;
	}
	hist_t hist = {move, captured_piece, side_castle, xside_castle, ep};
	move_history.push(hist);

	// make move
	color[move.from] = EMPTY;
	color[move.to] = side;
	pieces[move.to] = pieces[move.from];
	pieces[move.from] = _;

	// move rook if castling
	if (move.bits & 2) {
		if (side == WHITE) {
			if (move.to == 62) { // kingside
				color[63] = EMPTY;
				color[61] = side;
				pieces[63] = _;
				pieces[61] = R;
			} else { // queenside
				color[56] = EMPTY;
				color[59] = side;
				pieces[56] = _;
				pieces[59] = R;
			}
		} else {
			if (move.to == 57) { // kingside
				color[56] = EMPTY;
				color[58] = side;
				pieces[56] = _;
				pieces[58] = R;
			} else { // queenside
				color[63] = EMPTY;
				color[60] = side;
				pieces[63] = _;
				pieces[60] = R;
			}
		}
		side_castle = 0;
	}

	// update castling rights
	if (pieces[56] != R || color[56] != side) side_castle &= (side ? 2 : 1);
	if (pieces[63] != R || color[63] != side) side_castle &= (side ? 1 : 2);
	if (side == WHITE && (pieces[60] != K || color[60] != side)) side_castle = 0;
	if (side == BLACK && (pieces[59] != K || color[59] != side)) side_castle = 0;

	if (pieces[0] != R || color[0] != xside) xside_castle &= (side ? 1 : 2);
	if (pieces[7] != R || color[7] != xside) xside_castle &= (side ? 2 : 1);
	if (xside == WHITE && (pieces[3] != K || color[3] != xside)) xside_castle = 0;
	if (xside == BLACK && (pieces[4] != K || color[4] != xside)) xside_castle = 0;

	// promotion
	if (move.bits & 4) {
		pieces[move.to] = move.promote;
	}

	// en passant captures the pawn
	if (move.bits & 8) {
		color[move.to + 8] = EMPTY;
		pieces[move.to + 8] = _;
	}

	// moving pawn 2 squares forwards sets up en passant
	if (move.bits & 16) {
		ep = 63 - (move.from - 8);
	} else {
		ep = -1;
	}

	Board::rotateBoard();
	std::swap(side, xside);
	std::swap(side_castle, xside_castle);

	if (isCheck(xside)) {
		undoMove();
		return false;
	}

	return true;
}

void Board::undoMove() {
	Board::rotateBoard();
	std::swap(side, xside);
	std::swap(side_castle, xside_castle);

	hist_t hist = move_history.top();
	move_history.pop();

	side_castle = hist.side_castle;
	xside_castle = hist.xside_castle;
	ep = hist.ep;

	move_t move = hist.move;
	color[move.from] = side;
	color[move.to] = hist.capture == _ ? EMPTY : xside;
	pieces[move.from] = pieces[move.to];
	pieces[move.to] = hist.capture;

	// castle
	if (move.bits & 2) {
		if (move.from == 60 && move.to == 62) {
			color[61] = EMPTY;
			pieces[61] = _;
			color[63] = side;
			pieces[63] = R;
		} else if (move.from == 60 && move.to == 58) {
			color[59] = EMPTY;
			pieces[59] = _;
			color[56] = side;
			pieces[56] = R;
		} else if (move.from == 59 && move.to == 57) {
			color[58] = EMPTY;
			pieces[58] = _;
			color[56] = side;
			pieces[56] = R;
		} else if (move.from == 59 && move.to == 61) {
			color[60] = EMPTY;
			pieces[60] = _;
			color[63] = side;
			pieces[63] = R;
		}
	}

	// promotion
	if (move.bits & 4) {
		pieces[move.from] = P;
	}

	// en passant
	if (move.bits & 8) {
		pieces[move.to - 8] = P;
		color[move.to - 8] = xside;
	}
}

bool Board::isAttacked(int square, int attacking_side) {
	int piece;
	int attacked_square;
	for (int sq = 0; sq < 64; sq++) {
		if (color[sq] != attacking_side) continue;
		piece = pieces[sq];

		// Pawns
		if (piece == P) {
			std::array<int, 2> pawn_capture;
			if (attacking_side == side) pawn_capture = {-9, -11};
			else pawn_capture = {9, 11};
			// capture diagonally
			for (int offset : pawn_capture) {
				attacked_square = mailbox[mailbox64[sq] + offset];
				if (attacked_square == square) return true;
			}
		}

		// other pieces
		else {
			for (int offset : offsets[piece]) {
				for (int i = 1;; i++) {
					attacked_square = mailbox[mailbox64[sq] + i*offset];
					if (attacked_square == square) return true;
					if (attacked_square == -1) break; // can't move off board
					if (color[attacked_square] != EMPTY) break; // can't move past piece
					if (!slide[piece]) break; // stop if piece can't slide
				}
			}
		}
	}

	return false;
}

bool Board::isCheck(int checked_side) {
	int king_square = -1;
	for (int sq = 0; sq < 64; sq++) {
		if (color[sq] == checked_side && pieces[sq] == K) {
			king_square = sq;
			break;
		}
	}

	int checking_side = (checked_side == WHITE) ? BLACK : WHITE;
	return isAttacked(king_square, checking_side);
}

std::vector<move_t> Board::generateMoves() {
	std::vector<move_t> moves;
	int piece;
	int from, to;
	move_t move;
	for (int sq = 0; sq < 64; sq++) {
		if (color[sq] != side) continue;
		from = sq;
		piece = pieces[sq];
		
		// Pawns
		if (piece == P) {
			// move one square forward
			to = mailbox[mailbox64[sq] - 10];
			if (to != -1 && color[to] == EMPTY) {
				// promotes on back rank
				if (sq/8 == 1) {
					for (Piece piece : {N, B, R, Q}) {
						move = {from, to, piece, 4};
						moves.push_back(move);
					}
				} else {
					move = {from, to, _, 0};
					moves.push_back(move);
				}
				
				// moves two squares from second rank
				if (sq/8 == 6) {
					to = mailbox[mailbox64[sq] - 20];
					if (to != -1 && color[to] == EMPTY) {
						move = {from, to, _, 16};
						moves.push_back(move);
					}
				}
			}

			// capture diagonally
			for (int offset : {-9, -11}) {
				to = mailbox[mailbox64[sq] + offset];
				if (to == -1 || color[to] != xside) continue;
				
				// promotes on back rank
				if (sq/8 == 1) {
					for (Piece piece : {N, B, R, Q}) {
						move = {from, to, piece, 5};
						moves.push_back(move);
					}
				} else if (to == ep) {
					move = {from, to, _, 8};
					moves.push_back(move);
				} else {
					move = {from, to, _, 1};
					moves.push_back(move);
				}				
			}
		}

		// other pieces
		else {
			for (int offset : offsets[piece]) {
				for (int i = 1;; i++) {
					to = mailbox[mailbox64[sq] + i*offset];
					if (to == -1) break; // can't move off board
					if (color[to] == side) break; // can't capture own piece
					if (color[to] == xside) {
						move = {from, to, _, 1};
						moves.push_back(move);
						break; // can't slide past a capture
					}
					move = {from, to, _, 0};
					moves.push_back(move);
					if (!slide[piece]) break; // stop if piece can't slide
				}
			}
		}

		// castling
		if (piece == K && side_castle) {
			if (side == WHITE) {
				// kingside
				if ((side_castle & 1) && color[61] == EMPTY && color[62] == EMPTY &&
					!isAttacked(60, xside) && !isAttacked(61, xside) && !isAttacked(62, xside)) {
					to = 62; // g1
					move = {from, to, _, 2};
					moves.push_back(move);
				}

				// queenside
				if ((side_castle & 2) && color[57] == EMPTY && color[58] == EMPTY &&
					color[59] == EMPTY && !isAttacked(58, xside) && !isAttacked(59, xside) &&
					!isAttacked(60, xside)) {
					to = 58; // c1
					move = {from, to, _, 2};
					moves.push_back(move);
				}
			} else {
				// queenside
				if ((side_castle & 2) && color[60] == EMPTY && color[61] == EMPTY &&
					color[62] == EMPTY && !isAttacked(59, xside) && !isAttacked(60, xside) &&
					!isAttacked(61, xside)) {
					to = 61; // c8
					move = {from, to, _, 2};
					moves.push_back(move);
				}

				// kingside
				if ((side_castle & 1) && color[57] == EMPTY && color[58] == EMPTY &&
					!isAttacked(57, xside) && !isAttacked(58, xside) && !isAttacked(59, xside)) {
					to = 57; // g8
					move = {from, to, _, 2};
					moves.push_back(move);
				}
			}
		}
	}

	return moves;
}

void Board::display() {
	for (int sq = 0; sq < 64; sq++) {
		if (color[sq] == EMPTY) {
			std::cout << "*";
		} else {
			std::cout << displayPiece(pieces[sq], color[sq]);
		}

		if (sq % 8 == 7) {
			std::cout << "\n";
		}
	}
}

void Board::rotateBoard() {
	std::reverse(color.begin(), color.end());
	std::reverse(pieces.begin(), pieces.end());
}