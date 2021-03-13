#include <iostream>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <string>

#include "Board.h"
#include "Artificial_intelligence.h"

void pause()
{
	std::cout << "\nPlease enter any character to continue\n";
	char c;
	std::cin.get(c);
}

void print_instructions()
{
	std::cout << "Welcome to Noughts and Crosses\n"
		"\nWINNING CONDITIONS\n"
		"- Place three of your counters ('O' or 'X') in a line\n"
		"- The line can be horizontal, vertical, or diagonal\n"
		"\nHOW TO PLAY\n"
		"- You will be prompted to place your piece '>'\n"
		"- Enter the coordinates of the cell in which you would like to place it\n"
		"- e.g. B1 is the middle cell\n";
}

Position input_position(const Board& b)
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
		Position p((letter - 'A'), x);
		if (!is_empty(b, p)) {
			std::cout << " is already taken. Please try again.\n";
			continue;
		}
	
		// position is valid
		return p;
	}
}

Position get_next_move(const Board& b, Difficulty diff = Difficulty::Impossible)
{
	if (player(b) == b.first_player())
		return input_position(b);
	else
		return calculated_position(b, diff);
}

// Provides a status message from the perspective of player p
std::string game_status(Outcome s, Player p)
{
	std::ostringstream oss;
	switch (s)
	{
	case Outcome::Win:
		oss << "Player " << p << " has won!";
		break;
	case Outcome::Loss:
		oss << "Player " << opponent(p) << " has won!";
		break;
	case Outcome::Draw:
		oss << "You drew!";
		break;
	case Outcome::Undecided:
		oss << "The game is in progress";
		break;
	}

	return oss.str();
}

Difficulty input_difficulty()
{
	std::cout << "\nPlease select your difficulty level (1, 2, 3, or 4).\n"
				"1. Easy\n"
				"2. Medium\n"
				"3. Hard\n"
				"4. Impossible\n";

	while(true)
	{
		std::cout << '>';
		std::string input;
		std::getline(std::cin, input);
		switch (input.at(0))
		{
		case '1':
			return Difficulty::Easy;
		case '2':
			return Difficulty::Medium;
		case '3':
			return Difficulty::Hard;
		case '4':
			return Difficulty::Impossible;
		default:
			std::cout << input << " is invalid. Please try again.\n";
			break;
		}
	}

}

int main()
try
{
	print_instructions();
	auto diff = input_difficulty();
	Board board(Player::O);

	auto current_player = player(board);
	auto state = Outcome::Undecided;
	while (state == Outcome::Undecided)
	{
		std::cout << board;
		
		current_player = player(board);
		
		auto pos = get_next_move(board, diff);
		
		board = result(board, pos);
		
		state = get_outcome(board, current_player);
	}

	std::cout << board;
	std::cout << game_status(state, current_player);
	pause();
	return 0;
}
catch (const std::exception& e)
{
	std::cerr << "error: " << e.what() << '\n';
}
