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
		
		template <typename Grid>
		bool areAllSquaresSamePlayer(const Grid& grid, const std::vector<std::pair<int, int>>& squares) {
			auto first = squares.front();
			if (!grid.isSquareClaimed(first.first, first.second)) { return false; }
			auto player = grid.getPlayerForSquare(first.first, first.second);
			for (const auto& sq : squares) {
				if (grid.getPlayerForSquare(sq) != player) { return false; }
			}
			return true;
		}
		
		template<typename Grid>
		std::pair<bool, Player> checkRows(const Grid& grid) {
			for (int row = 0; row < grid.getRows(); ++row) {
				if (!grid.isSquareClaimed(row, 0)) { continue; }
				auto player = grid.getPlayerForSquare(row, 0);
				for (int col = 1; col < grid.getColumns(); ++col) {
					if (!grid.isSquareClaimed(row, col) || grid.getPlayerForSquare(row, col) != player) {
						break;
					}
				}
				return std::make_pair(true, player);
			}
			return std::make_pair(true, Player());
		}
		
		template<typename Grid>
		std::pair<bool, Player> checkColumns(const Grid& grid);
		
		template<typename Grid>
		std::pair<bool, Player> checkDiagonals(const Grid& grid);
		
	};

}

#endif
