#include <fstream>
#include <vector>
#include <string>

#include "catch.hpp"
#include "../src/position.hpp"
#include "../src/move.hpp"
#include "../src/init.hpp"


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


TEST_CASE("Move Generation") {

/*
1. Load in FENs

For each FEN:

2. Load into Position

3. Generate MoveList via our game protocol

4. Rearrange MoveList so that it is an 81 x 16 array, with each inner array containing moves for every start square at index startsquare

5. Generate moves for each square manually via otf ops

For each Square:

6. initial check that moveLists are the same size

For each otf Move:

7. encode move, assert membership in rearranged MoveList.

*/
    init_table();

    std::string filename = "tests/test_fens.txt";

    std::vector<std::string> fens = load_test_fens(filename);

    Position pos;

    for (std::string fen: fens) {
        pos.set(fen);
        std::cout << "FEN loaded: " << fen << '\n';  // Verify FEN loaded
        print_position(pos);

        MoveList moveList(pos);

        Move indexedMoveList[81][16] = {};

        for (const auto& move : moveList) {
            Square from_sq = move.from();
            for (int i = 0; i < 16; ++i) {
                if (!indexedMoveList[from_sq][i]) {
                    indexedMoveList[from_sq][i] = move;
                    break;
                }
            }
        }

        Move otfMoves[81][16] = {};
        Bitboard occupied = pos.occupied();

        if (pos.side_to_move() == Attackers) {
            Bitboard fromBB = pos.attackers();
            while (fromBB) {
                Square from_sq = fromBB.bitscan_pop_forward();
                Bitboard otfBB = find_moves_otf(occupied, from_sq);
                otfBB &= (~occupied & THRONE_OUT_MASK);
                while (otfBB) {
                    Square to_sq = otfBB.bitscan_pop_forward();
                    Move otf_move = encodeMove(Attacker, from_sq, to_sq);
                    for (int i = 0; i < 16; ++i) {
                        if (!otfMoves[from_sq][i]) {
                            otfMoves[from_sq][i] = otf_move;
                            break;
                        }
                    }
                }
            }
        }

        else {
            Bitboard fromBB = pos.defenders();
            while (fromBB) {
                Square from_sq = fromBB.bitscan_pop_forward();
                Bitboard otfBB = find_moves_otf(occupied, from_sq);
                otfBB &= (~occupied & THRONE_OUT_MASK);
                while (otfBB) {
                    Square to_sq = otfBB.bitscan_pop_forward();
                    Move otf_move = encodeMove(Defender, from_sq, to_sq);
                    for (int i = 0; i < 16; ++i) {
                        if (!otfMoves[from_sq][i]) {
                            otfMoves[from_sq][i] = otf_move;
                            break;
                        }
                    }
                }
            }

            fromBB = pos.king();
            Square from_sq = pos.king_index();
            Bitboard otfBB = find_moves_otf(occupied, from_sq);
            otfBB &= ~occupied;
            while (otfBB) {
                Square to_sq = otfBB.bitscan_pop_forward();
                Move otf_move = encodeMove(King, from_sq, to_sq);
                for (int i = 0; i < 16; ++i) {
                    if (!otfMoves[from_sq][i]) {
                        otfMoves[from_sq][i] = otf_move;
                        break;
                    }
                }
            }
        }

        for (Square sq = SQA1; sq < SquareNum; ++sq) {
            int num_magic_moves = 0;
            int num_otf_moves = 0;

            for (Move move : indexedMoveList[sq]) {
                if (move) {
                    num_magic_moves++;
                }
            }

            for (Move move : otfMoves[sq]) {
                if (move) {
                    num_otf_moves++;
                }
            }
            REQUIRE(num_magic_moves == num_otf_moves);

            for (Move otf_move : otfMoves[sq]) {
                bool present_flag = false;
                if (otf_move) {
                    for (Move magic_move : indexedMoveList[sq]) {
                        if (magic_move.value() == otf_move.value()) {
                            present_flag = true;
                            break;
                        }
                    }
                    REQUIRE(present_flag);
                }
            }
        }
    }
}
