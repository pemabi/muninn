# pragma once

#include <iostream>
#include <string>

#include "move.hpp"
#include "position.hpp"
#include "common.hpp"
#include "UCTSearch.hpp"

static bool is_valid_format(const std::string& move) {
  if (move.length() != 5) return false;
  if (move[2] != '-') return false;

  if (move[0] < 'a' || move[0] > 'i') return false;
  if (move[1] < '1' || move[1] > '9') return false;
  if (move[3] < 'a' || move[3] > 'i') return false;
  if (move[4] < '1' || move[4] > '9') return false;

  return true;
}

static PieceType get_piece_type_at(const Position& pos, Square from, Side side) {
    if (side == Attackers) {
        if (pos.attacker_bb() & SquareMaskBB[from]) {
            return Attacker;
        }
    } else {
        if (pos.defender_bb() & SquareMaskBB[from]) {
            return Defender;
        } else if (pos.king_bb() & SquareMaskBB[from]) {
            return King;
        }
    }
    return PieceNum; // invalid case
}

class Agent {
public:
    static Move random(const BoardHistory& bh) {
        MoveList moves(bh.current_pos());
        if (moves.empty()) {
            std::cout<<"no move found\n";  // this case should only happen if intial position has no moves
            return moveNone();
        }
        Move move = moves.get_move_by_index(RandomGenerator::random_int(0, moves.size() - 1));
        return move;
    }

    static Move greedy() {return moveNone();}  // can add simple loop that iterates thru movelist, makes moves, counts bits / looks at captures / checks win

    static Move rl_training(const BoardHistory& bh) {
        // no transposition tables, etc, so think that stack allocation would be fine, but copying this approach
        // important to create new search between moves because tree reuse here can affect noise
        auto uct = std::make_unique<UCTSearch>(bh.shallow_clone());
        return uct->search(bh.shallow_clone());
    }

    // Take user moves in format a1-a2
    static Move player(const BoardHistory& bh) {
        MoveList moves(bh.current_pos());
        if (moves.empty()) {
          std::cout<<"no move found\n";
          return moveNone();
        }

        std::string player_move;
        bool valid_move = false;

        while (!valid_move) {
            std::cout<<"Enter Move: ";
            std::cin>>player_move;

            if (!is_valid_format(player_move)) {
                std::cout<<"Invalid format. Example move 'a1-a2'\n";
                continue;
            }

            int from_file = player_move[0] - 'a';
            int from_rank = player_move[1] - '1';
            int to_file = player_move[3] - 'a';
            int to_rank = player_move[4] - '1';

            Square from = index_square(static_cast<File>(from_file), static_cast<Rank>(from_rank));
            Square to = index_square(static_cast<File>(to_file), static_cast<Rank>(to_rank));

            Side side = bh.current_pos().side_to_move();
            PieceType pt = get_piece_type_at(bh.current_pos(), from, side);

            if (pt == PieceNum) {
                std::cout<<"Invalid move.\n";
                continue;
            }

            Move move = encodeMove(pt, from, to);

            if (moves.contains(move)) {
                return move;
            }

            std::cout<<"Invalid move.\n";
        };

        return moveNone();
    }
};


namespace gameWrapper {
  // add in agents here, separate agent namespace perhaps

void loop();

}
