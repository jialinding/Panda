#include <cstdlib>
#include <algorithm>

#include "utils.h"

int parseInput(std::string input, const std::vector<move_t>& moves) {
	int from = 64 - 8*atoi(input.substr(1,1).c_str()) + (input[0] - 'a');
	int to = 64 - 8*atoi(input.substr(3,1).c_str()) + (input[2] - 'a');

	int move_index;
	for (move_index = 0; move_index < moves.size(); ++move_index) {
		if (moves[move_index].from == from && moves[move_index].to == to) {
			if (input.length() == 5) {
				char piece = input[4];
				switch (piece) {
					case 'N': return move_index;
					case 'B': return move_index + 1;
					case 'R': return move_index + 2;
					case 'Q': return move_index + 3;
				}
			}

			return move_index;
		}
	}

	return -1;
}

std::string displayPiece(Piece piece, int color) {
	std::string output;
	switch (piece) {
		case 0: output = "P"; break;
		case 1: output = "N"; break;
		case 2: output = "B"; break;
		case 3: output = "R"; break;
		case 4: output = "Q"; break;
		case 5: output = "K"; break;
	}

	if (color == WHITE) {
		std::transform(output.begin(), output.end(), output.begin(), ::tolower);
	}

	return output;
}