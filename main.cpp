#include <iostream>
#include <vector>

#include "board.h"
#include "utils.h"
#include "defs.h"

int main(int argc, char const *argv[])
{
	Board board;
	
	std::string input;
	move_t move;
	srand(time(NULL));
	bool valid_move = true;
	while (true) {
		// computer move
		if (board.side == BLACK) {
			std::vector<move_t> moves = board.generateMoves();
			int randomIndex = rand() % moves.size();
			move = moves[randomIndex];
			board.move(move);
		}

		// user move
		else {
			board.display();
			if (!valid_move) std::cout << "Please enter a valid move" << std::endl;
			std::cout << ">> ";
			std::cin >> input;
			if (input == "q") break;
			std::vector<move_t> moves = board.generateMoves();
			int move_index = parseInput(input, moves);
			if (move_index == -1) valid_move = false;
			else valid_move = board.move(moves[move_index]);
		}
	}
	return 0;
}