#include "Artificial_intelligence.h"

#include <stdexcept>
#include <string>
#include <limits>
#include <random>
#include <chrono>



int utility(const Board& b, Player maximising_player)
{
	switch (get_outcome(b, maximising_player)) {
	case Outcome::Win:
		return 10;
	case Outcome::Loss:
		return -10;
	case Outcome::Draw:
		return 0;
	default:
		throw std::invalid_argument("b must be in a terminal state (win, loss, or draw)");
		break;
	}
}

bool is_terminal(const Board& b)
{
	// The state is terminal if there is a win, a loss, or
	// a draw (for any player). This is equivalent to the 
	// state not being undecided.
	return get_outcome(b, Player::X) != Outcome::Undecided;
}

// bool is_terminal(Outcome s)
// {
// 	switch (s) {
// 	case Outcome::Draw:
// 	case Outcome::Win:
// 	case Outcome::Loss:
// 		return true;
// 	default:
// 		return false;
// 	}
// }

/// <summary>
/// Determines the utility of state s, which MUST be a terminal state
/// </summary>
/// <param name="s">A terminal state</param>
/// <returns>The utility of s</returns>
// int utility(Outcome s)
// {
// 	switch (s) {
// 	case Outcome::Win:
// 		return 1;
// 		break;
// 	case Outcome::Loss:
// 		return -1;
// 		break;
// 	case Outcome::Draw:
// 		return 0;
// 		break;
// 	case Outcome::Undecided:
// 	default:
// 		throw std::invalid_argument("b must be in a terminal state (win, loss, or draw)");
// 		break;
// 	}
// }

//int utility(const Board& b, Player first_player)
//{
//	auto state = determine_state(b, first_player);
//	switch (state)
//	{
//	case State::Win:
//		return 10;
//		break;
//	case State::Loss:
//		return -10;
//		break;
//	case State::Draw:
//		return 0;
//		break;
//	case State::Undecided: default:
//		throw std::invalid_argument("b must be in a terminal state (win, loss, or draw)");
//		break;
//	}
//}

//int min_value(const Board& b);
//
///// <summary>
///// Determines the maximum value that could be obtained from the boards that result from placing MIN's counter on all of the blank spaces
///// </summary>
///// <param name="b"></param>
///// <returns></returns>
//int max_value(const Board& b)
//{
//	auto s = determine_state(b, b.first_player());
//	if(is_terminal(s))
//		return utility(s);
//
//	auto current_max = std::numeric_limits<int>::lowest();
//	// For each blank space, choose the maximum from the highest utility seen so far
//	// and the highest utility that could result from MIN's moves
//	for (const auto& action : actions(b))
//		current_max = std::max(current_max, min_value(result(b, action)));
//
//	return current_max;
//}
//
//int min_value(const Board& b)
//{
//	auto s = determine_state(b, b.first_player());
//	if (is_terminal(s))
//		return utility(s);
//
//	auto current_min = std::numeric_limits<int>::max();
//
//	// Choose the smallest utility of the boards that result from MAX
//	// playing the next turn
//	//
//	// For each blank space, choose the minimum from the current lowest
//	// value and the maximum value that MAX could obtain by taking
//	// each action (i.e. placing his counter on the blank spaces)
//
//	for (const auto& action : actions(b))
//		current_min = std::min(current_min, max_value(result(b, action)));
//
//	return current_min;
//}

int min_value(Board& b, Player maximising_player);

/// <summary>
/// Determines the maximum value that could be obtained from the boards that result from placing MIN's counter on all of the blank spaces
/// </summary>
/// <param name="b">board</param>
/// <returns></returns>
int max_value(Board& b, Player maximising_player)
{
	// auto s = get_outcome(b, opponent(b.first_player()));
	// if (is_terminal(s))
	// 	return utility(s);

	if (is_terminal(b))
		return utility(b, maximising_player);

	auto current_max = std::numeric_limits<int>::lowest();
	// For each blank space, choose the maximum from the highest utility seen so far
	// and the highest utility that could result from MIN's moves
	for (auto& action : actions(b)) {
		Counter counter = player(b) == Player::X ? Counter::X : Counter::O;
		b.set(action, counter);
		current_max = std::max(current_max, min_value(b, maximising_player));
		b.set(action, Counter::None);
	}

	return current_max;
}

int min_value(Board& b, Player maximising_player)
{
	// auto s = get_outcome(b, opponent(b.first_player()));
	// if (is_terminal(s))
	// 	return utility(s);

	if (is_terminal(b))
		return utility(b, maximising_player);

	auto current_min = std::numeric_limits<int>::max();

	// Choose the smallest utility of the boards that result from MAX
	// playing the next turn
	//
	// For each blank space, choose the minimum from the current lowest
	// value and the maximum value that MAX could obtain by taking
	// each action (i.e. placing his counter on the blank spaces)

	for (const auto& action : actions(b)) {
		Counter counter = player(b) == Player::X ? Counter::X : Counter::O;
		b.set(action, counter);
		current_min = std::min(current_min, max_value(b, maximising_player));
		b.set(action, Counter::None);
	}
	return current_min;
}

Pos minimax_position(const Board& b)
{
	// The minimax algorithm (AI player) always takes the role of MAX
	// so it wants to obtain the highest score (utility) possible.

	// The action that will (eventually) give the highest utility
	auto best_action = actions(b)[0];
	auto current_max = std::numeric_limits<int>::min();

	for (const auto& action : actions(b)) {
		Counter counter = player(b) == Player::X ? Counter::X : Counter::O;

		// v is the smallest utility of the boards
		// that result from performing the given action

		Board board = b;
		// Consider the possible utility for the current action
		board.set(action, counter);
		auto v = min_value(board, player(b));
		if (v > current_max) {
			best_action = action;
			current_max = v;
		}
		// Remove the counter placed on the board
		board.set(action, Counter::None);
	}

	return best_action;

	// Return the action that

	// Minimax_player is always the maximising player,
	// so choose the board with the greatest value that could
	// result from one of MIN's moves

	// Determine the smallest value
	//auto min_utility = min_value(b);

	// Work out which board corresponds to the
	// min_utility. If there are multiple boards with min_utility,
	// then choose the first one in the list of actions
}

int rand_int(int min, int max)
{
	// Doesn't work with MinGW64
	//std::random_device rd;

	// Seed the RNG
	std::default_random_engine gen{ std::chrono::system_clock::now().time_since_epoch().count() };
	std::uniform_int_distribution<int> dist{ min, max };
	return dist(gen);
}

Pos random_position(const Board& b)
{
	bool is_valid = false;
	auto valid_moves = actions(b);

	auto i = rand_int(0, valid_moves.size() - 1);
	return valid_moves[i];
}

Pos calculated_position(const Board& b, Difficulty diff)
{
	auto per_cent = rand_int(0, 100);

	switch (diff) {
	case Difficulty::Easy:
		return random_position(b);
		break;
	case Difficulty::Medium:
		if (per_cent <= (int)Difficulty::Medium)
			return minimax_position(b);
		else
			return random_position(b);
	case Difficulty::Hard:
		if (per_cent <= (int)Difficulty::Hard)
			return minimax_position(b);
		else
			return random_position(b);
	case Difficulty::Impossible:
		return minimax_position(b);
	default:
		throw std::invalid_argument("Pos calculated_position(const Board& b, Difficulty diff): Unsupported difficulty");
		break;
	}
}
