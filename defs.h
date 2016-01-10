#ifndef DEFS_H
#define DEFS_H

#include <array>
#include <vector>

#define WHITE 0
#define BLACK 1
#define EMPTY 2

#define EXACT_NODE 0
#define ALPHA_NODE 1
#define BETA_NODE 2

enum Piece {P, N, B, R, Q, K, _};

/* Bits:
   1 - capture
   2 - castle
   4 - promote
   8 - en passant
   16 - pawn moves 2 squares */
struct move_t {
	int from;
	int to;
	Piece promote;
	int bits; // metadata about move
};

struct hist_t {
	move_t move;
	Piece capture;
	int side_castle;
	int xside_castle;
	int ep;
};

struct tt_entry {
	move_t best_move;
	int depth;
	int score;
	int node_type;
};

/* Board */
extern std::array<int, 64> init_color;
extern std::array<Piece, 64> init_pieces;

extern std::vector<std::vector<int> > offsets;
extern std::vector<bool> slide;

extern std::array<int, 120> mailbox;
extern std::array<int, 64> mailbox64;

/* Evaluation */
extern std::vector<int> piece_values;
extern std::array<int, 64> pawn_pcsq;
extern std::array<int, 64> knight_pcsq;
extern std::array<int, 64> bishop_pcsq;
extern std::array<int, 64> king_pcsq;

#endif