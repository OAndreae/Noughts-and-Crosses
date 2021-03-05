#include <iostream>
#include <string>

#include "Board.h"

Board::Board(Player first_player) : m_first_player{first_player}
{
	for (auto& c : m_board)
		c = Counter::None;
}

Counter Board::at(unsigned int column, unsigned int row) const
{
	return m_board.at(to_index(column, row));
}

Counter Board::at(Pos p) const
{
	return at(p.column, p.row);
}

bool Board::set(unsigned int column, unsigned int row, Counter c)
{
	m_board[to_index(column, row)] = c;

	return is_empty(*this, column, row);
}

bool Board::set(Pos p, Counter c)
{
	return set(p.column, p.row, c);
}

std::ostream& operator<<(std::ostream& os, const Board& b)
{
	/*	- - - - - - -
		:   :   :   :
		- - - - - - -
		:   :   :   :
		- - - - - - -
		:   :   :   :
		- - - - - - -
	*/

	using namespace std;

	static const string line{ "- - - - - - -" };
	static const string row { "-\n-\n-\n" };
	static const char separator = ':';
	static const string indent = "\t";


	os << '\n'
		<< indent << "    A   B   C\n"
		<< indent << "  " + line << "\n";

	for (size_t row = 0; row < b.rows(); row++) {
		os << indent << row << " ";
		for (size_t column = 0; column < b.columns(); column++) {
			os << separator << " " << (char)b.at(column, row) << " ";
		}
		os << separator << '\n' << 
			indent << "  " + line << '\n';
	}

	return os << '\n';
}

bool is_empty(const Board& b, Pos p)
{
	return is_empty(b, p.column, p.row);
}

bool is_empty(const Board& b, unsigned int column, unsigned int row)
{
	return b.at(column, row) == Counter::None;
}

bool is_full(const Board& b)
{
	for (const auto& c : b)
		if (c == Counter::None)
			return false;
	
	return true;
}

// Returns the blank positions on board b.
// These are the 'actions' that can be performed.
std::vector<Pos> actions(const Board& b)
{
	std::vector<Pos> empty_spaces;
	for (unsigned int i = 0; i < b.rows(); i++)
		for (unsigned int j = 0; j < b.columns(); j++)
			if (b.at(i, j) == Counter::None)
				empty_spaces.push_back(Pos{ i,j });

	return empty_spaces;
}


bool has_won(const Board& b, Player p)
{
	auto counter = p == Player::X ? Counter::X : Counter::O;

	// three in a column
	if (b.at(0, 0) == counter && b.at(0, 1) == counter && b.at(0, 2) == counter)
		return true;
	if (b.at(1, 0) == counter && b.at(1, 1) == counter && b.at(1, 2) == counter)
		return true;
	if (b.at(2, 0) == counter && b.at(2, 1) == counter && b.at(2, 2) == counter)
		return true;
	// three in a row
	if (b.at(0, 0) == counter && b.at(1, 0) == counter && b.at(2, 0) == counter)
		return true;
	if (b.at(0, 1) == counter && b.at(1, 1) == counter && b.at(2, 1) == counter)
		return true;
	if (b.at(0, 2) == counter && b.at(1, 2) == counter && b.at(2, 2) == counter)
		return true;
	// leading diagonal
	if (b.at(0, 0) == counter && b.at(1, 1) == counter && b.at(2, 2) == counter)
		return true;
	// opposite diagonal
	if (b.at(2, 0) == counter && b.at(1, 1) == counter && b.at(0, 2) == counter)
		return true;

	return false;
}

State determine_state(const Board& b, Player p)
{
	if (is_full(b))
		return State::Draw;

	if (has_won(b, p))
		return State::Win;
	else if (has_won(b, opponent(p))) // zero-sum game: if X wins, the O loses and vice versa
		return State::Loss;

	// Not a draw, win, or loss: must be in progress and Undecided
	return State::Undecided;
}

/// <summary>
/// Defines the board that results from placing the current player's counter 
/// at position p on board b
/// </summary>
/// <param name="b">The board on which to place the counter</param>
/// <param name="p">The position to place the counter</param>
/// <returns>The board that results from placing the current player's counter at 
/// position p on board b</returns>
Board result(const Board& b, const Pos& p)
{
	Counter counter = player(b) == Player::X ? Counter::X : Counter::O;
	Board board = b;
	board.set(p, counter);
	return board;
}

/// <summary>
/// Identifies the player whose turn it is in board b
/// </summary>
/// <param name="b">A Board</param>
/// <returns>The player whose turn it is in board b</returns>
Player player(const Board& b)
{
	auto counters_played = 0;
	for (const auto& c : b)
		if (c != Counter::None)
			counters_played++;

	auto second_player = b.first_player() == Player::X ? Player::O : Player::X;

	if (counters_played % 2 == 0)
		return b.first_player();
	else
		return second_player;
}

/// <summary>
/// Determines the opponent (e.g. if p is O, X is opponent)
/// </summary>
/// <param name="p">The current player</param>
/// <returns>The opposing player</returns>
Player opponent(Player p)
{
	return p == Player::X ? Player::O : Player::X;
}

/// <summary>
/// Outputs player p as 'X' or 'O'
/// </summary>
/// <param name="os">The output stream</param>
/// <param name="p">The player to output</param>
/// <returns></returns>
std::ostream& operator<<(std::ostream& os, Player p)
{
	if (p == Player::X)
		return os << 'X';
	else
		return os << 'O';
}
