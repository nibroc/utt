#include "TicTacToeGrid.h"

#include <iostream>

int main() {
	utt::TicTacToeGrid<int, 3, 3> grid;
	
	grid.claimSquare(0, 0, 0);
	grid.claimSquare(0, 1, 1);
	grid.claimSquare(0, 2, 2);
	
	std::cout << grid.isFinished() << std::endl;
	
	if (grid.isFinished()) {
		std::cout << "Player: " << grid.getWinner() << " won " << std::endl;
	}
	
}
