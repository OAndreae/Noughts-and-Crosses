#include "Artificial_intelligence.h"

#include <stdexcept>
#include <string>
#include <limits>
#include <random>
#include <chrono>

// Determines the score of the terminal state b
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

// Returns true if b is in a win, loss, or draw state
bool is_terminal(const Board& b)
{
	// The state is terminal if there is a win, a loss, or
	// a draw (for any player). This is equivalent to the 
	// state not being undecided.
	return get_outcome(b, Player::X) != Outcome::Undecided;
}

int min_value(Board& b, Player maximising_player);

// Determines the maximum value that could be obtained from the boards
// that result from placing MIN's counter on all of the blank spaces
int max_value(Board& b, Player maximising_player)
{
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

// Calculates the optimal position on which to place a
// counter. Optimal players either win or draw; they don't 
// lose.
Position minimax_position(const Board& b)
{
	// The minimax algorithm (AI player) always takes the role of MAX
	// so it wants to obtain the highest score (utility) possible.

	// The action that will (eventually) give the highest utility
	auto best_action = actions(b)[0];
	auto current_max = std::numeric_limits<int>::min();

	// Determine the action with the highest utility
	for (const auto& action : actions(b)) {
		Counter counter = player(b) == Player::X ? Counter::X : Counter::O;

		// Copy so that b can remain const
		Board board = b;

		// Perform the action and update the best action if
		// it has a better (higher) utility
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
}

int rand_int(int min, int max)
{
	// Doesn't work with MinGW64
	//std::random_device rd;

	// Seed the RNG
	long seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine gen{ seed };
	std::uniform_int_distribution<int> dist{ min, max };
	return dist(gen);
}

// Returns a random position from the blank spaces
// on board b
Position random_position(const Board& b)
{
	auto valid_moves = actions(b);

	auto i = rand_int(0, valid_moves.size() - 1);
	return valid_moves[i];
}

// Higher difficulties increase the probability of an optimal (minimax) position
// being returned. Non-optimal moves are random.
Position calculated_position(const Board& b, Difficulty diff)
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