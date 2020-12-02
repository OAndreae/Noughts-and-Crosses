#include "Minimax.h"

#include <stdexcept>
#include <limits>

bool is_terminal(State s)
{
	switch (s) {
	case State::Draw: case::State::Win: case State::Loss:
		return true;
	default:
		return false;
	}
}

/// <summary>
/// Determines the utility of state s, which MUST be a terminal state
/// </summary>
/// <param name="s">A terminal state</param>
/// <returns>The utility of s</returns>
int utility(State s)
{
	switch (s)
	{
	case State::Win:
		return 10;
		break;
	case State::Loss:
		return -10;
		break;
	case State::Draw:
		return 0;
		break;
	case State::Undecided: default:
		throw std::invalid_argument("b must be in a terminal state (win, loss, or draw)");
		break;
	}
}


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

int min_value(Board& b);

/// <summary>
/// Determines the maximum value that could be obtained from the boards that result from placing MIN's counter on all of the blank spaces
/// </summary>
/// <param name="b"></param>
/// <returns></returns>
int max_value(Board& b)
{
	auto s = determine_state(b, b.first_player());
	if (is_terminal(s))
		return utility(s);

	auto current_max = std::numeric_limits<int>::lowest();
	// For each blank space, choose the maximum from the highest utility seen so far
	// and the highest utility that could result from MIN's moves
	for (auto& action : actions(b)) {
		Counter counter = player(b) == Player::X ? Counter::X : Counter::O;
		b.set(action, counter);
		current_max = std::max(current_max, min_value(b));
		b.set(action, Counter::None);
	}

	return current_max;
}

int min_value(Board& b)
{
	auto s = determine_state(b, b.first_player());
	if (is_terminal(s))
		return utility(s);

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
		current_min = std::min(current_min, max_value(b));
		b.set(action, Counter::None);
	}
	return current_min;
}




Pos minimax_position(const Board& b)
{
	auto best_action = actions(b)[0];
	auto current_max = std::numeric_limits<int>::min();

	for (const auto& action : actions(b))
	{
		Counter counter = player(b) == Player::X ? Counter::X : Counter::O;

		// v is the smallest utility of the boards
		// that result from performing the given action
		Board board = b;
		board.set(action, counter);
		auto v = min_value(board);
		if (v > current_max)
		{
			best_action = action;
			current_max = v;
		}
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