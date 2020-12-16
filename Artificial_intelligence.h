#pragma once
#include "Board.h"

// The percentage chance that an optimal move will be played
enum class Difficulty 
{
    Easy = 0,
    Medium = 30,
    Hard = 80,
    Impossible = 100
};

Pos minimax_position(const Board& b);

Pos random_position(const Board& b);

Pos calculated_position(const Board& b, Difficulty diff);