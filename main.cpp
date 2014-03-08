#include "TicTacToeGrid.h"

#include <iostream>

int main() {
	utt::TicTacToeGrid<int, 3, 3> grid;
	std::cout << grid.isFinished() << std::endl;
}
