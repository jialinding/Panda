#include <iostream>
#include <vector>

#include "board.h"
#include "utils.h"
#include "defs.h"
#include "search.h"

int main(int argc, char const *argv[])
{
	Board board;
	Search search;
	
	std::string input;
	move_t move;
	srand(time(NULL));
	bool valid_move = true;
	while (true) {
		// computer move
		if (board.side == BLACK) {
			move_t computer_move = search.think(board, 3);
			board.move(computer_move);
		}

		// user move
		else {
			board.display();
			if (!valid_move) std::cout << "Please enter a valid move" << std::endl;
			std::cout << ">> ";
			std::cin >> input;
			if (input == "q") break;
			if (input == "n") {
				board = Board();
				continue;
			}
			std::vector<move_t> moves = board.generateMoves();
			int move_index = parseInput(input, moves);
			if (move_index == -1) valid_move = false;
			else valid_move = board.move(moves[move_index]);
		}

		if (printResult(board)) break;
	}
	return 0;
}