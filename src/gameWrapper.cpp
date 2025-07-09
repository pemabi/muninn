
#include <iostream>

#include "position.hpp"
#include "gameWrapper.hpp"

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
            return ~bh.current_pos().side_to_move();
        }
        // check for surrounding wins from Attackers
        if (bh.current_pos().side_to_move() == Defenders) {
            if (bh.current_pos().is_surrounded(moves_next.all_to_squares())) {  // if no overlap between edge squares and all 'to' squares, flood fill
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
