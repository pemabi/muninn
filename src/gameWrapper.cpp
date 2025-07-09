#include "position.hpp"
#include "gameWrapper.hpp"

#include <iostream>
#include <string>

using AgentFunction = std::function<Move(const BoardHistory&)>;

namespace gameWrapper {

int play_one_game(BoardHistory& bh, AgentFunction attackerAgent, AgentFunction defenderAgent) {
    for (int game_ply = 0; game_ply < 400; ++game_ply) {
        print_position(bh.current_pos());
        Move move;
        if (bh.current_pos().side_to_move() == Attackers) {
            move = attackerAgent(bh);
        }
        else {
            move = defenderAgent(bh);
        }

        bh.do_move(move);

        if (bh.current_pos().winner() != sideNum) {  // if the move is a king capture / king escape
            print_position(bh.current_pos());
            return bh.current_pos().winner();
        }

        // check for win by repetitions. As this can only be claimed by the Defender, only checking this after Attacker moves
        if (bh.current_pos().side_to_move() == Defenders) {
            int reps = bh.current_pos().repetitions_count();
            if (reps == 3) {
                print_position(bh.current_pos());
                return Attackers;
            }
        }

        MoveList moves_next(bh.current_pos());  // generating next moves a) to ensure that opponent can move b) more effecient surrounding win check

        // if player can't move, opponent wins
        if (moves_next.size() == 0) {
            print_position(bh.current_pos());
            return ~bh.current_pos().side_to_move();
        }
        // check for surrounding wins from Attackers
        if (bh.current_pos().side_to_move() == Defenders) {
            if (bh.current_pos().is_surrounded(moves_next.all_to_squares())) {  // if no overlap between edge squares and all 'to' squares, flood fill
                print_position(bh.current_pos());
                return Attackers;
            }
        }
    }
    return sideNum;  // in event that no winner is found after max game plies
}

int play_one_game(AgentFunction attackerAgent, AgentFunction defenderAgent) {
    BoardHistory bh;
    bh.set(Position::StartFEN);
    std::cout<<"Board History set\n";
    bh.current_pos().initZobrist();
    int game_result = play_one_game(bh, attackerAgent, defenderAgent);
    return game_result;
}

void loop() {
    int result = play_one_game(Agent::player, Agent::player);
    std::cout<<result<<" wins\n";
}

}

// Helper functions for validating user inputted moves
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

Move Agent::random(const BoardHistory& bh) {
    MoveList moves(bh.current_pos());
    if (moves.empty()) {
        std::cout<<"no move found\n";  // this case should only happen if intial position has no moves
        return moveNone();
    }
    Move move = moves.get_move_by_index(RandomGenerator::random_int(0, moves.size() - 1));
    return move;
}

Move Agent::greedy(const BoardHistory& bh) {
    return moveNone();
}

Move Agent::rl_training(const BoardHistory& bh) {
    // no transposition tables, etc, so think that stack allocation would be fine, but copying this approach
    // important to create new search between moves because tree reuse here can affect noise
    auto uct = std::make_unique<UCTSearch>(bh.shallow_clone());
    return uct->search(bh.shallow_clone());
}

// takes user inputted moves in format 'a1-a2'
Move Agent::player(const BoardHistory& bh) {
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

    return moveNone();  // should never reach here
}
