#include <iostream>
#include <vector>
#include <boost/tokenizer.hpp>

#include "board.h"
#include "utils.h"
#include "defs.h"
#include "search.h"

void pick_side(int& human_side, int& computer_side);

// UCI compliant
int main2(int argc, char const *argv[])
{
	Board board;
	Search search;
	
	std::string input;
	std::string startpos_moves;
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
int main(int argc, char const *argv[])
{
	Board board;
	Search search;
	
	std::string input;
	srand(time(NULL));
	bool valid_move = true;

	// determine which side to play
	int human_side, computer_side;
	pick_side(human_side, computer_side);

	// play game
	while (true) {
		// computer move
		if (board.side == computer_side) {
			move_t computer_move = search.think(board, 2);  // TODO: change depth
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
				pick_side(human_side, computer_side);
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

void pick_side(int& human_side, int& computer_side) {
	std::string input;
	while (true) {
		std::cout << "Which side would you like to play (w or b)? ";
		std::cin >> input;
		if (input == "w") {
			human_side = WHITE;
			computer_side = BLACK;
			break;
		} else if (input == "b") {
			human_side = BLACK;
			computer_side = WHITE;
			break;
		}
		std::cout << "Please enter a valid color" << std::endl;
	}
}