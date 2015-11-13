#include <cstdlib>
#include <algorithm>

#include "utils.h"

move_t parseInput(std::string input) {
	move_t move;
	move.from = 64 - 8*atoi(input.substr(1,1).c_str()) + (input[0] - 'a');
	move.to = 64 - 8*atoi(input.substr(3,1).c_str()) + (input[2] - 'a');
	return move;
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