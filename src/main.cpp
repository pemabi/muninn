#include <fstream>
#include <vector>
#include <string>

#include "init.hpp"
#include "magics.hpp"

#include "position.hpp"
#include "move.hpp"

std::vector<std::string> load_test_fens(const std::string& filename) {
    std::vector<std::string> fens;
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Could not open file " + filename);
    }

    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty() && line[0] != '/') {
            fens.push_back(line);
        }
    }
    return fens;
}

int main() {
#ifdef FIND_MOVE_MAGICS
    find_all_move_magics();
#endif

#ifdef FIND_ATTACK_MAGICS
    find_all_attack_magics();
#endif

#ifdef DEBUG_MODE

#endif

#ifdef TEST_MODE

    RandomGenerator rng;

    init_table();

    Position pos;
    //print_position(pos);

    pos.set("3AAA3/4A4/1A7/A3KD1A1/A5D1A/A1D4DA/2A1D4/3A5/4AA3 d 1");
    print_position(pos);

    MoveList moves(pos);
    std::cout<<moves.size()<<'\n';

    for (const auto& move : moves) {
        std::cout << move.movedPiece() << " " << move.from() << " " << move.to() <<'\n';
    }


#endif
    return 0;
}
