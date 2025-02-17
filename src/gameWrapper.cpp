
#include <iostream>

#include "position.hpp"

namespace gameWrapper {

using AgentFunction = std::function<Move(BoardHistory, RandomGenerator)>;

int play_one_game(BoardHistory& bh, AgentFunction attackerAgent, AgentFunction defenderAgent) {
    RandomGenerator rng;
    for (int game_ply = 0; game_ply < 300; ++game_ply) {
        print_position(bh.current_pos());
        std::cout<<bh.current_pos().winner()<<'\n';
        Move move;
        if (bh.current_pos().side_to_move() == Attackers) {
            move = attackerAgent(bh.shallow_clone(), rng);
        }
        else {
            move = defenderAgent(bh.shallow_clone(), rng);
        }
        std::cout<<"about to do move\n";
        bh.do_move(move); // need to add something in move / here to guard against no possible move scenarios
        std::cout<<"Move made...\n";
        if (bh.current_pos().winner() != sideNum) {
            print_position(bh.current_pos());
            return bh.current_pos().winner();
        }
        // check move list, check for surrounds
        //MoveList moves(bh.current_pos());
    }
    return sideNum;
}

int play_one_game(AgentFunction attackerAgent, AgentFunction defenderAgent) {
    BoardHistory bh;
    bh.set(Position::StartFEN);
    std::cout<<"Board History set\n";
    int game_result = play_one_game(bh, attackerAgent, defenderAgent);
    return game_result;

}

Move random_agent(BoardHistory bh, RandomGenerator rng) {
    MoveList moves(bh.current_pos());
    std::cout<<"Moves found...\n";
    Move move = moves.get_move_by_index(rng.random_int(0, moves.size()-1));
    std::cout<<"Move selected...\n";
    return move;
}

void loop() {
    int result = play_one_game(random_agent, random_agent);
    std::cout<<result<<" wins\n";
}

}
