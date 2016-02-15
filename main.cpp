#include <iostream>
#include <vector>
#include <boost/tokenizer.hpp>

#include "board.h"
#include "utils.h"
#include "defs.h"
#include "search.h"

// UCI compliant
int main(int argc, char const *argv[])
{
	Board board;
	Search search;
	
	std::string input;
	std::string startpos_moves;
	move_t move;
	srand(time(NULL));
	while (std::getline(std::cin, input)) {
		if (input == "uci") {
			std::cout << "id name Panda 1.0" << std::endl;
			std::cout << "id author Jialin Ding" << std::endl;
			std::cout << "uciok" << std::endl;
		} else if (input == "quit") {
			break;
		} else if (input == "isready") {
			std::cout << "readyok" << std::endl;
		} else if (input.substr(0, 24) == "position startpos moves ") {
			board = Board();
			boost::char_separator<char> sep(" ");
			startpos_moves = input.substr(24);
			boost::tokenizer<boost::char_separator<char> > tokens(startpos_moves, sep);
			for (std::string move : tokens) {
				std::vector<move_t> moves = board.generateMoves();
				int move_index = parseInput(move, board.side, moves);
				if (move_index != -1) board.move(moves[move_index]);
			}
		} else if (input.substr(0, 3) == "go ") {
			move_t computer_move = search.think(board, 4);
			std::cout << "bestmove " << parseMove(computer_move, board.side) << std::endl;
		} else if (input == "board") { // debugging
			board.display();
		}
	}
	return 0;
}

// main function when playing from terminal/command line
int main2(int argc, char const *argv[])
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
			move_t computer_move = search.think(board, 4);
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
			int move_index = parseInput(input, board.side, moves);
			if (move_index == -1) valid_move = false;
			else valid_move = board.move(moves[move_index]);
		}

		if (printResult(board)) break;
	}
	return 0;
}