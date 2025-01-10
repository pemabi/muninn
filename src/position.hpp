#pragma once

#include "pieces.hpp"
#include "common.hpp"
#include "bitboard.hpp"

#include <stack>

class Position;

enum GameResult : int8_t  {   // no draws in the game - maybe add win type for more info?
    AttackerWin, DefenderWin, GameResultNum
};

struct StateInfo {
    int plies_from_null;

    //Key board_key;

    StateInfo* previous;

    //ChangedLists cl;

    //Key key() const { return board_key; }
};

// smart pointer to manage a double ended queue of state info
using StateListPtr = std::unique_ptr<std::deque<StateInfo>>;

/*
Should add in some kind of compressing technique here (Huffman is obvious, custom ANS?) for ML export. Position, eval, etc.
Consider however the tensor stack - last few positions will be stored. should these all be encoded together? Should i not bother with encoding?
 */

class Move;
struct Thread;
struct Searcher;    // Think I might start with implementing a naive agent

class Position  {
public:
    static void init();

    // representation of inital position
    //static const CodedPos

    Position() = default;


};
