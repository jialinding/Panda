#include "defs.h"

std::array<int, 64> init_color = {
	1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1,
	2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0
};

std::array<Piece, 64> init_pieces = {
	R, N, B, Q, K, B, N, R,
	P, P, P, P, P, P, P, P,
	_, _, _, _, _, _, _, _,
	_, _, _, _, _, _, _, _,
	_, _, _, _, _, _, _, _,
	_, _, _, _, _, _, _, _,
	P, P, P, P, P, P, P, P,
	R, N, B, Q, K, B, N, R
};

std::vector<std::vector<int> > offsets = {
	{},
	{-21, -19, -12, -8, 8, 12, 19, 21},
	{-11, -9, 9, 11},
	{-10, -1, 1, 10},
	{-11, -10, -9, -1, 1, 9, 10, 11},
	{-11, -10, -9, -1, 1, 9, 10, 11}
};

std::vector<bool> slide = {false, false, true, true, true, false};

std::array<int, 120> mailbox = {
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1,  0,  1,  2,  3,  4,  5,  6,  7, -1,
	-1,  8,  9, 10, 11, 12, 13, 14, 15, -1,
	-1, 16, 17, 18, 19, 20, 21, 22, 23, -1,
	-1, 24, 25, 26, 27, 28, 29, 30, 31, -1,
	-1, 32, 33, 34, 35, 36, 37, 38, 39, -1,
	-1, 40, 41, 42, 43, 44, 45, 46, 47, -1,
	-1, 48, 49, 50, 51, 52, 53, 54, 55, -1,
	-1, 56, 57, 58, 59, 60, 61, 62, 63, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1
};

std::array<int, 64> mailbox64 = {
	21, 22, 23, 24, 25, 26, 27, 28,
	31, 32, 33, 34, 35, 36, 37, 38,
	41, 42, 43, 44, 45, 46, 47, 48,
	51, 52, 53, 54, 55, 56, 57, 58,
	61, 62, 63, 64, 65, 66, 67, 68,
	71, 72, 73, 74, 75, 76, 77, 78,
	81, 82, 83, 84, 85, 86, 87, 88,
	91, 92, 93, 94, 95, 96, 97, 98
};