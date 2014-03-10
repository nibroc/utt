#include "TicTacToeGrid.h"

#include "utestpp/utestpp.h"

#include <cassert>
#include <iostream>
#include <string>

utt::TicTacToeGrid<int> gridFromString(const std::string& str) {
	assert(str.size() <= 9);
	utt::TicTacToeGrid<int> g;
	for (auto i = 0u; i < str.length(); ++i) {
		if (str[i] == '-') { continue; }
		const int row = i / 3;
		const int col = i % 3;
		const int player = (str[i] == 'X') ? 0 : 1;
		g.claimSquare(player, row, col);
	}
	return g;
}

void testSimpleWin() {
	utt::TicTacToeGrid<int> grid;
	
	UTESTPP_ASSERT_TRUE(!grid.isFinished());
	UTESTPP_ASSERT_TRUE(grid.isSquareAvailable(0, 0));
	
	grid.claimSquare(0, 0, 0);
	UTESTPP_ASSERT_TRUE(!grid.isSquareAvailable(0, 0));
	
	grid.claimSquare(0, 1, 1);
	grid.claimSquare(0, 2, 2);
	
	UTESTPP_ASSERT_TRUE(grid.isFinished());
	UTESTPP_ASSERT_EQUAL(grid.getWinner(), 0);
}

void testSimpleGame1() {
	auto grid = gridFromString("XXO" "XXO" "X--");
	UTESTPP_ASSERT_TRUE(grid.isFinished());
	UTESTPP_ASSERT_EQUAL(grid.getWinner(), 0);
}

void testSimpleGame2() {
	auto grid = gridFromString("OOX" "OOX" "O--");
	UTESTPP_ASSERT_TRUE(grid.isFinished());
	UTESTPP_ASSERT_EQUAL(grid.getWinner(), 1);
}

void testUnfinishedGame() {
	auto grid = gridFromString("XXO");
	UTESTPP_ASSERT_TRUE(!grid.isFinished());
	UTESTPP_ASSERT_THROWS(grid.getWinner());
	UTESTPP_ASSERT_THROWS(grid.getPlayerForSquare(2, 2));
}

void testRowWin() {
	auto grid = gridFromString("XO-" "-OO" "XXX");
	UTESTPP_ASSERT_EQUAL(grid.getWinner(), 0);
}

void testColumnWin() {
	auto g1 = gridFromString("XO-" "XO-" "-O-");
	UTESTPP_ASSERT_EQUAL(g1.getWinner(), 1);
	
	auto g2 = gridFromString("XO-" "XO-" "X--");
	UTESTPP_ASSERT_EQUAL(g2.getWinner(), 0);
	
	auto g3 = gridFromString("XOX" "XOX" "OOX");
	UTESTPP_ASSERT_EQUAL(g3.getWinner(), 1);
}

void testDiagonalWin() {
	auto g1 = gridFromString("XOO" "OXO" "OXX");
	UTESTPP_ASSERT_EQUAL(g1.getWinner(), 0);
	
	auto g2 = gridFromString("-XO" "XO-" "O--");
	UTESTPP_ASSERT_EQUAL(g2.getWinner(), 1);
}

int main() {
	UTESTPP_INIT();
	
	testSimpleWin();
	
	testSimpleGame1();
	testSimpleGame2();
	
	testUnfinishedGame();
	
	testRowWin();
	testColumnWin();
	testDiagonalWin();
	
	UTESTPP_FINISH();
}
