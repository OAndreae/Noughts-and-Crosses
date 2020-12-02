#include <iostream>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <string>

#include "Board.h"
#include "Minimax.h"

void pause()
{
	std::cout << "Please enter any character to continue\n";
	char c;
	std::cin.get(c);
}

void print_instructions()
{
	std::cout << "Welcome to Noughts and Crosses\n"
		"\nWINNING CONDITIONS\n"
		"- Place three of your counters ('O' or 'X') in a line\n"
		"- This can be either in a line, column or diagonal\n"
		"\nHOW TO PLAY\n"
		"- You will be prompted to place your piece '>'\n"
		"- Enter the coordinates of the cell in which you would like to place it\n"
		"- e.g. b2 is the middle cell\n";
}

Pos input_position(const Board& b)
{
	std::cout << "Player " << player(b) << "'s turn:\n";

	while (true)
	{
		std::cout << '>';

		std::string input;
		std::getline(std::cin, input);

		std::string invalid_input_message = input + " is not a valid position. Please try again.\n";
		// check if sufficient characters
		if (input.size() != 2) {
			std::cout << invalid_input_message;
			continue;
		}

		// check if format is valid
		auto letter = input.at(0);
		auto num = input.at(1);
		if (!std::isalpha(letter) || !std::isdigit(num)) {
			std::cout << invalid_input_message;
			continue;
		}

		// check if postion is on the board
		letter = std::toupper(letter);
		auto x = num - '0';
		if (!(letter == 'A' || letter == 'B' || letter == 'C') || x < 0 || x > b.columns()-1) {
			std::cout << invalid_input_message;
			continue;
		}

		// check if position is blank
		Pos p((letter - 'A'), x);
		if (!is_empty(b, p)) {
			std::cout << " is already taken. Please try again.\n";
			continue;
		}
	
		// position is valid
		return p;
	}
}

Pos get_next_move(const Board& b)
{
	if (player(b) == b.first_player())
		return minimax_position(b);
	else
		return input_position(b);
}

//Player player(const Board& b)
//{
//	auto x_counters = 0;
//	auto o_counters = 0;
//	for (const auto& c : b)
//	{
//		if (c == Counter::X)
//			x_counters++;
//		else if (c == Counter::O)
//			o_counters++;
//	}
//
//	if (x_counters == 0 && o_counters == 0)
//		return first_player();
//	else if (x_counters > o_counters) // X has had more turns than O, so it's O's turn
//		return Player::O;
//	else
//		return Player::X;
//}

void print(const std::vector<Pos>& actions)
{
	for (auto& position : actions)
	{
		auto letter = 'X';
		switch (position.column)
		{
		case 0:
			letter = 'A';
		case 1:
			letter = 'B';
		case 2:
			letter = 'C';
		default:
			letter = 'X';
		}

		std::cout << letter << position.row << "\n";
	}
}
void print(State s)
{
	switch (s)
	{
	case State::Win:
		std::cout << "win\n";
		break;
	case State::Loss:
		std::cout << "loss\n";
		break;
	case State::Draw:
		std::cout << "draw\n";
		break;
	case State::Undecided:
		std::cout << "undecided\n";
		break;
	default:
		break;
	}
}

/// <summary>
/// Provides a status message from the perspective of player p
/// </summary>
/// <param name="s">The state of the game</param>
/// <param name="p">The player whose turn it is</param>
/// <returns>A commentator's message on the game's state</returns>
std::string game_status(State s, Player p)
{
	std::ostringstream oss;
	switch (s)
	{
	case State::Win:
		oss << "Player " << p << " has won!";
		break;
	case State::Loss:
		oss << "Player " << opponent(p) << " has won!";
		break;
	case State::Draw:
		oss << "You drew!";
		break;
	case State::Undecided:
		oss << "The game is in progress";
		break;
	}

	return oss.str();
}

int main()
try
{
	Board board(Player::X);

	auto current_player = player(board);
	auto state = State::Undecided;
	while (state == State::Undecided)
	{
		std::cout << board;
		
		current_player = player(board);
		
		auto pos = get_next_move(board);
		
		board = result(board, pos);
		
		state = determine_state(board, current_player);
	}

	std::cout << board;
	std::cout << game_status(state, current_player);
	return 0;
}
catch (const std::exception& e)
{
	std::cerr << "error: " << e.what() << '\n';
}