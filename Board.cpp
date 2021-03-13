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

Counter Board::at(Position p) const
{
	return at(p.column, p.row);
}

bool Board::set(unsigned int column, unsigned int row, Counter c)
{
	m_board[to_index(column, row)] = c;

	return is_empty(*this, column, row);
}

bool Board::set(Position p, Counter c)
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

bool is_empty(const Board& b, Position p)
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
std::vector<Position> actions(const Board& b)
{
	std::vector<Position> empty_spaces;
	for (unsigned int i = 0; i < b.rows(); i++)
		for (unsigned int j = 0; j < b.columns(); j++)
			if (b.at(i, j) == Counter::None)
				empty_spaces.push_back(Position{ i,j });

	return empty_spaces;
}

// Determines whether player p has won the game on board b
bool has_won(const Board& b, Player p)
{
	auto counter = p == Player::X ? Counter::X : Counter::O;

	// For each column, check for three adjacent counters of the same type
	for (unsigned int c = 0; c < b.columns(); ++c) {
		for (unsigned int r = 0; r < b.rows() - 2; ++r) {
			// Three in column found if next two counters are of the same type
			if (b.at(c, r) == counter && b.at(c, r + 1) == counter && b.at(c, r + 2) == counter)
				return true;
		}
	}

	// For each row, check for three adjacent counters of the same type
	for (unsigned int r = 0; r < b.rows();++r) {
		for (unsigned int c = 0; c < b.columns() - 2; ++c) {
			// Three in a row found if next two counters are of the same type
			if (b.at(c, r) == counter && b.at(c + 1, r) == counter && b.at(c + 2, r) == counter)
				return true;
		}
	}

	// Check the leading diagonals ('\')
	for (unsigned int c = 0; c < b.columns() - 2;++c) {
		for (unsigned int r = 0; r < b.rows() - 2;++r) {
			if (b.at(c, r) == counter && b.at(c + 1, r + 1) == counter && b.at(c + 2, r + 2) == counter)
				return true;
		}
	}

	// Check the non-leading diagonals ('/')
	for (unsigned int c = b.columns()-1; c > 1; --c) {
		for (unsigned int r = 0; r < b.rows() - 2; ++r) {
			if (b.at(c, r) == counter && b.at(c - 1, r + 1) == counter && b.at(c - 2, r + 2) == counter)
				return true;
		}
	}

	// Player p hasn't won: the game is either in progress (undecided), drawn, or lost
	return false;
}

// Determines the outcome of board b from the perspective
// of player p. Returns Outcome::Undecided if b isn't terminal.
Outcome get_outcome(const Board& b, Player p)
{
	if (has_won(b, p))
		return Outcome::Win;
	else if (has_won(b, opponent(p)))
		return Outcome::Loss;
	else if (actions(b).size() == 0)
		return Outcome::Draw;

	// The winning conditions were not found and the board isn't full
	return Outcome::Undecided;
}

// Returns the board that results from placing the current player's counter 
// at position p on board b
Board result(const Board& b, const Position& p)
{
	Counter counter = player(b) == Player::X ? Counter::X : Counter::O;
	Board board = b;
	board.set(p, counter);
	return board;
}

// Identifies the player whose turn it is in board b
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

// Determines the opponent (e.g. if p is O, X is opponent)
Player opponent(Player p)
{
	return p == Player::X ? Player::O : Player::X;
}

// Outputs player p as 'X' or 'O'
std::ostream& operator<<(std::ostream& os, Player p)
{
	if (p == Player::X)
		return os << 'X';
	else
		return os << 'O';
}
