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
	
	namespace detail {

		template<int v>
		struct AreTemplateArgumentsEqualHelper {
			typedef std::false_type value;
		};

		template<>
		struct AreTemplateArgumentsEqualHelper<0> {
			typedef std::true_type value;
		};

		template<int x, int y>
		struct AreTemplateArgumentsEqual {
			typedef typename AreTemplateArgumentsEqualHelper<x - y>::value value;
		};

		template<typename Player, bool isSquare>
		struct TicTacToeWinCheckerSquareHelperBool;

		template<typename Player>
		struct TicTacToeWinCheckerSquareHelperBool<Player, true> {
			typedef TicTacToeWinCheckerSquare<Player> checker;
		};

		template <typename Player, int rows, int columns> 
		struct TicTacToeWinCheckerSquareHelper {
			typedef typename TicTacToeWinCheckerSquareHelperBool<
				Player, 
				std::is_same<
					std::integral_constant<int, rows>, 
					std::integral_constant<int, columns>
				>::value
			>::checker checker;
		};
		
	}

	template<typename Player, 
		int Rows = 3, int Columns = 3, 
		typename WinChecker = typename detail::TicTacToeWinCheckerSquareHelper<
			Player, Rows, Columns
		>::checker
	>
	class TicTacToeGrid {
	public:

		TicTacToeGrid(const WinChecker& checker = WinChecker())
		  : finished(false), grid(), winner(), checker(checker)
		{ }
		
		void claimSquare(const Player& player, int row, int column) {
			grid[row][column].setPlayer(player);
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
		
		const Player& getPlayerForSquare(int row, int column) {
			if (!isSquareClaimed(row, column)) {
				throw std::runtime_error("Board does not have a player in square");
			}
			return grid[row][column].player;
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

			void setPlayer(const Player& player) {
				player = player;
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
			for (int row = 0; row < grid.rows(); ++row) {
				if (!grid.isSquareClaimed(row, 0)) { continue; }
				auto player = grid.getPlayerForSquare(row, 0);
				for (int col = 1; col < grid.cols(); ++grid) {
					if (!grid.isSquareClaimed(row, col) || grid.getPlayerForSquare(row, col) != player) {
						break;
					}
				}
			}
		
		}
		template<typename Grid>
		std::pair<bool, Player> checkColumns(const Grid& grid);
		
		template<typename Grid>
		std::pair<bool, Player> checkDiagonals(const Grid& grid);
		
	};

}

#endif
