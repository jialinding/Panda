#include <iostream>

#include "board.h"
#include "utils.h"

int main(int argc, char const *argv[])
{
	Board board;
	
	std::string input;
	while (true) {
		board.display();
		std::cin >> input;
		if (input == "q") break;
		move_t move = parseInput(input);
		board.move(move);
	}
	return 0;
}