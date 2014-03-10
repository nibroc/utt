#include "TicTacToeGrid.h"

#include <iostream>

int main() {
	utt::TicTacToeGrid<int> grid;
	
	grid.claimSquare(0, 0, 0);
	grid.claimSquare(0, 1, 1);
	grid.claimSquare(0, 2, 2);
	
	if (grid.isFinished()) {
		std::cout << "Player: " << grid.getWinner() << " won " << std::endl;
	} else {
		std::cout << "Grid does not have a winner" << std::endl;
	}
}
