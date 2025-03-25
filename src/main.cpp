#include <fstream>
#include <vector>
#include <string>

#include "init.hpp"
#include "magics.hpp"
#include "gameWrapper.hpp"

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



#endif
    return 0;
}
