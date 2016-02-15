#include <cstdlib>
#include <algorithm>
#include <iostream>

#include "utils.h"

int parseInput(std::string input, int side, const std::vector<move_t>& moves) {
	if (input.size() < 4) return -1;

	int from = 64 - 8*atoi(input.substr(1,1).c_str()) + (input[0] - 'a');
	int to = 64 - 8*atoi(input.substr(3,1).c_str()) + (input[2] - 'a');

	if (side == BLACK) {
		from = 63 - from;
		to = 63 - to;
	}

	int move_index;
	for (move_index = 0; move_index < moves.size(); ++move_index) {
		// std::cout << moves[move_index].from << moves[move_index].to << std::endl;
		if (moves[move_index].from == from && moves[move_index].to == to) {
			if (input.length() == 5) {
				char piece = input[4];
				switch (piece) {
					case 'n': return move_index;
					case 'b': return move_index + 1;
					case 'r': return move_index + 2;
					case 'q': return move_index + 3;
				}
			}

			return move_index;
		}
	}

	return -1;
}

// Takes a move_t and returns in UCI format
std::string parseMove(move_t move, int side) {
	if (side == BLACK) {
		move.from = 63 - move.from;
		move.to = 63 - move.to;
	}
	std::string ret;
	ret += (char)('a' + (move.from % 8));
	ret += std::to_string(8 - (move.from / 8));
	ret += (char)('a' + (move.to % 8));
	ret += std::to_string(8 - (move.to / 8));
	if (move.promote != _) {
		switch (move.promote) {
			case N: ret += 'n';
			case B: ret += 'b';
			case R: ret += 'r';
			case Q: ret += 'q';
		}
	}
	return ret;
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

/* Currently only checks checkmate and stalemate */
bool printResult(Board& board) {
	for (move_t& move : board.generateMoves()) {
		if (board.move(move)) {
			board.undoMove();
			return false;
		}
	}

	board.display();
	if (board.isCheck(board.side)) {
		std::cout << (board.side ? "1 - 0" : "0 - 1") << std::endl; 
	} else {
		std::cout << "0.5 - 0.5" << std::endl;
	}
	return true;
}