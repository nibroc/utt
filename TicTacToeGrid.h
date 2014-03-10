#ifndef TICTACTOEGRID_H
#define	TICTACTOEGRID_H

#include <array>
#include <stdexcept>
#include <utility>
#include <type_traits>
#include <vector>

namespace utt {

	template<typename Player>
	struct TicTacToeWinCheckerSquare;

	template<typename Player, 
		int Rows = 3, int Columns = 3,
		typename WinChecker = typename std::enable_if<
			Rows == Columns, TicTacToeWinCheckerSquare<Player>
		>::type
	>
	class TicTacToeGrid {
	public:

		TicTacToeGrid(const WinChecker& checker = WinChecker())
		  : finished(false), grid(), winner(), checker(checker)
		{ }
		
		void claimSquare(const Player& player, int row, int column) {
			grid[row][column].setPlayer(player);
			auto check = checker(*this);
			if (check.first) {
				finished = true;
				winner = check.second;
			}
		}

		bool isSquareClaimed(int row, int column) const {
			return grid[row][column].isClaimed();
		}

		bool isSquareAvailable(int row, int column) const {
			return !isSquareClaimed(row, column);
		}

		bool isFinished() const {
			return finished;
		}
		
		bool isTie() const {
			return isTie;
		}
		
		const Player& getPlayerForSquare(int row, int column) const {
			if (!isSquareClaimed(row, column)) {
				throw std::runtime_error("Board does not have a player in square");
			}
			return grid[row][column].getPlayer();
		}

		const Player& getWinner() const {
			if (!isFinished()) {
				throw std::runtime_error("Board does not have a winner");
			}
			return winner;
		}

		bool isWinner(const Player& player) const {
			return getWinner() == player;
		}
		
		int getRows() const {
			return Rows;
		}
		
		int getColumns() const {
			return Columns;
		}

	private:

		class Square {
		public:

			Square() = default;

			explicit Square(const Player& player) : claimed(true), player(player)
			{ }

			bool isClaimed() const {
				return claimed;
			}

			const Player& getPlayer() const {
				return player;
			}
			
			void setPlayer(const Player& play) {
				player = play;
				claimed = true;
			}

		private:
			bool claimed;
			Player player;
		};

		bool finished;
		std::array<std::array<Square, Columns>, Rows>  grid;
		Player winner;
		WinChecker checker;
	};

	template<typename Player>
	struct TicTacToeWinCheckerSquare {
		template<typename Grid>
		std::pair<bool, Player> operator()(const Grid& grid) {
			auto rowWin = checkRows(grid);
			if (rowWin.first) {
				return rowWin;
			}
			
			auto colWin = checkColumns(grid);
			if (colWin.first) {
				return colWin;
			}
			
			auto diagonalWin = checkDiagonals(grid);
			if (diagonalWin.first) {
				return diagonalWin;
			}
			
			return std::make_pair(false, Player());
		}
		
	private:
		
		template<typename Grid>
		bool isColumnSamePlayer(const Grid& g, int col) {
			if (!g.isSquareClaimed(0, col)) { return false; }
			const auto& player = g.getPlayerForSquare(0, col);
			for (int r = 0; r < g.getRows(); ++r) {
				if (!g.isSquareClaimed(r, col)) { return false; }
				if (g.getPlayerForSquare(r, col) != player) { return false; }
			}
			return true;
		}
		
		template<typename Grid>
		bool isRowSamePlayer(const Grid& g, int row) {
			if (!g.isSquareClaimed(row, 0)) { return false; }
			const auto& player = g.getPlayerForSquare(row, 0);
			for (int c = 0; c < g.getColumns(); ++c) {
				if (!g.isSquareClaimed(row, c)) { return false; }
				if (g.getPlayerForSquare(row, c) != player) { return false; }
			}
			return true;
		}
		
		template<typename Grid>
		bool isTopLeftToBottomRightDiagonalSamePlayer(const Grid& g) {
			if (!g.isSquareClaimed(0, 0)) { return false; }
			const auto& player = g.getPlayerForSquare(0, 0);
			for (int pos = 1; pos < g.getRows(); ++pos) {
				if (!g.isSquareClaimed(pos, pos)) { return false; }
				if (g.getPlayerForSquare(pos, pos) != player) { return false; }
			}
			return true;
		}
		
		template<typename Grid>
		bool isBottomLeftToTopRightDiagonalSamePlayer(const Grid& g) {
			if (!g.isSquareClaimed(0, g.getColumns() - 1)) { return false; }
			const auto& player = g.getPlayerForSquare(0, g.getColumns() - 1);
			for (int row = 1; row < g.getRows(); ++row) {
				int col = g.getColumns() - row - 1;
				if (!g.isSquareClaimed(row, col)) { return false; }
				if (g.getPlayerForSquare(row, col) != player) { return false; }
			}
			return true;
		}
		
		template<typename Grid>
		std::pair<bool, Player> checkRows(const Grid& grid) {
			for (int row = 0; row < grid.getRows(); ++row) {
				if (isRowSamePlayer(grid, row)) {
					return std::make_pair(true, grid.getPlayerForSquare(row, 0));
				}
			}
			return std::make_pair(false, Player());
		}
		
		template<typename Grid>
		std::pair<bool, Player> checkColumns(const Grid& grid) {
			for (int col = 0; col < grid.getColumns(); ++col) {
				if (isColumnSamePlayer(grid, col)) {
					return std::make_pair(true, grid.getPlayerForSquare(0, col));
				}
			}
			return std::make_pair(false, Player());
		}
		
		template<typename Grid>
		std::pair<bool, Player> checkDiagonals(const Grid& grid) {
			if (isTopLeftToBottomRightDiagonalSamePlayer(grid)) {
				return std::make_pair(true, grid.getPlayerForSquare(0, 0));
			}
			if (isBottomLeftToTopRightDiagonalSamePlayer(grid)) {
				return std::make_pair(true, grid.getPlayerForSquare(grid.getRows() - 1, 0));
			}
			return std::make_pair(false, Player());
		}
		
	};

}

#endif
