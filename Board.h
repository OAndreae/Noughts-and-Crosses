#pragma once
#include <vector>
#include <stdexcept>

enum class Counter : char
{
	None = ' ',
	X = 'X',
	O = 'O'
};

struct Position
{
	Position(unsigned int c, unsigned int r) : column{c}, row{r}{}
	unsigned int column;
	unsigned int row;
};

enum class Player
{
	X,
	O
};

class Board
{
public:
	Board(Player first_player);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="i">The column [0, 2]</param>
	/// <param name="j">The row [0, 2]</param>
	/// <returns>The counter in position (<paramref name="i"/>, <paramref name="j"/>)</returns>
	Counter at(unsigned int column, unsigned int row) const;
	Counter at(Position p) const;
	
	/// <summary>
	/// Places counter <paramref name="c"/> on space (<paramref name="i"/>, <paramref name="j"/>) if 
	/// (<paramref name="i"/>, <paramref name="j"/>) is blank
	/// </summary>
	/// <param name="i">The column [0, 2]</param>
	/// <param name="j">The row [0, 2]</param>
	/// <param name="c">The counter to place at (<paramref name="i"/>, <paramref name="j"/>)</param>
	/// <returns>True if (<paramref name="i"/>, <paramref name="j"/>) was blank, otherwise false</returns>
	bool set(unsigned int column, unsigned int row, Counter c);
	bool set(Position p, Counter c);

	Player first_player() const { return m_first_player; }

	unsigned int rows() const { return 3; }
	unsigned int columns() const { return 3; }


	size_t size() { return m_board.size(); };

	std::vector<Counter>::iterator begin() { return m_board.begin(); }
	std::vector<Counter>::const_iterator begin() const { return m_board.begin(); }
	std::vector<Counter>::iterator end() { return m_board.end(); }
	std::vector<Counter>::const_iterator end() const { return m_board.end(); }

private:
	unsigned int to_index(unsigned int column, unsigned int row) const
	{
		if (column > 2 || column < 0)
			throw std::out_of_range(std::string{ column + " is out of range [0, 2]" });
		else if (row > 2 || row < 0)
			throw std::out_of_range(std::string{ row + " is out of range [0, 2]" });

		// 0 1 2
		// 3 4 5
		// 6 7 8
		return (columns() * row) + column;
	}
private:
	std::vector<Counter> m_board{ 9 };
	Player m_first_player;
};

std::ostream& operator<<(std::ostream& os, const Board& b);

bool is_empty(const Board& b, unsigned int column, unsigned int row);
bool is_empty(const Board& b, Position p);
/// <summary>
/// Determines whether the given <c>Board</c> is full of counters
/// </summary>
/// <param name="b">The board</param>
/// <returns>True if the board is full; <c>false</c> if the board has (a) space(s)</returns>
bool is_full(const Board& b);



/// <summary>
/// Determines the which player's turn it is;
/// precondition: no spaces on the board have been overwritten 
/// </summary>
/// <param name="b">The board</param>
/// <returns>The player whose turn it is</returns>
Player player(const Board& b);
Player opponent(Player p);
std::ostream& operator<<(std::ostream& os, Player p);
std::vector<Position> actions(const Board& b);
Board result(const Board& b, const Position& p);

enum class Outcome
{
	Win,
	Loss,
	Draw,
	Undecided
};

// Determines the outcome of board b from the perspective
// of player p. Returns Outcome::Undecided if b isn't terminal.
Outcome get_outcome(const Board& b, Player p);

