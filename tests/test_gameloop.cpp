#include <fstream>
#include <vector>
#include <string>
#include <sstream>

#include "catch.hpp"
#include "../src/position.hpp"
#include "../src/move.hpp"
#include "../src/init.hpp"

TEST_CASE("Game Loop") {
    init_table();

    std::string filename = "data/games_by_move.txt";
    std::ifstream file(filename);
    REQUIRE(file.is_open());

    std::string line;
    Position pos;
    pos.set(Position::StartFEN);
    int game_counter = 0;

    while (std::getline(file, line)) {
        if (line.empty()) continue;  // Skip empty lines

        if (line == "#") {
            pos.set(Position::StartFEN);  // Reset position for new game
            game_counter += 1;
            std::cout<<"processing game "<<game_counter<<" | move ";
            continue;
        }

        std::istringstream iss(line);
        int from, to;
        int move_counter = 0;
        while (iss >> from >> to) {
            bool move_found = false;
            Move played_move;

            MoveList moveList(pos);
            for (const auto& move : moveList) {
                Square from_sq = move.from();
                Square to_sq = move.to();
                if (static_cast<int>(from_sq) == from && static_cast<int>(to_sq) == to) {
                    played_move = move;
                    move_found = true;
                    std::cout<<move_counter<<" ";
                    break;  // Exit loop once move is found
                }
            }
            if (!move_found) {
                std::cout<<"\nFAILED on move "<<move_counter<<" | "<<from<<" "<<to;
            }

            REQUIRE(move_found);  // Test that the move was legal
            pos.do_move(played_move);
            move_counter+=1;
            if (game_counter == 27) {
                print_position(pos);
            }
        }
        std::cout<<'\n';
    }
}
