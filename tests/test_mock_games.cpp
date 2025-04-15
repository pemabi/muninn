#include "catch.hpp"

#include "../src/init.hpp"
#include "../src/position.hpp"

int play_one_game(BoardHistory& bh, const std::vector<Move>& moves_played) {
    static RandomGenerator rng; // would have to change this to run games in parallel
    for (Move my_move : moves_played) {
        print_position(bh.current_pos());
        MoveList moves(bh.shallow_clone().current_pos());
        Move move = moveNone();
        for (const auto& m : moves) {
            if (m == my_move) {
                move = m;
                break;
            }
        }
        if (!move) {
            print_position(bh.current_pos());
            return -1;
        }
        bh.do_move(move);
        if (bh.current_pos().winner() != sideNum) {
            print_position(bh.current_pos());
            return bh.current_pos().winner();
        }

        // check for win by repetitions. As this can only be claimed by the Defender, want to make sure this only happens after an attacker move
        if (bh.current_pos().side_to_move() == Defenders) {
            int reps = bh.current_pos().repetitions_count();
            std::cout<<reps<<" repetitions\n";
            if (reps == 3) {
                print_position(bh.current_pos());
                return Attackers;
            }
        }

        // check move list, check for surrounds.
        MoveList moves_next(bh.current_pos());
        // if player can't move, opponent wins
        if (moves_next.size() == 0) {
            print_position(bh.current_pos());
            return ~bh.current_pos().side_to_move();
        }
        // check for surrounding wins from Attackers
        if (bh.current_pos().side_to_move() == Defenders) {
            if (bh.current_pos().is_surrounded(moves_next.all_to_squares())) {
                print_position(bh.current_pos());
                return Attackers;
            }
        }
    }
    return sideNum;
}

int play_one_game(const std::vector<Move>& moves_played) {
    BoardHistory bh;
    bh.set(Position::StartFEN);
    int game_result = play_one_game(bh, moves_played);
    std::cout<<game_result<<'\n';
    return game_result;
}

TEST_CASE("Game Loop") {

    // Defender win, escape
    std::vector<Move> moves_played_1 = {encodeMove(Attacker, SQA4, SQA3), encodeMove(Defender, SQE6, SQG6),
        encodeMove(Attacker, SQI6, SQI9), encodeMove(King, SQE5, SQE6), encodeMove(Attacker, SQA6, SQA9),
        encodeMove(King, SQE6, SQA6)
        };
    int result_1 = 0;

    // Defender win, escape 2
    std::vector<Move> moves_played_2 = {encodeMove(Attacker, SQA6, SQA8), encodeMove(Defender, SQD5, SQD8),
        encodeMove(Attacker, SQA8, SQC8), encodeMove(King, SQE5, SQD5), encodeMove(Attacker, SQD1, SQC1),
        encodeMove(King, SQD5, SQD1)
        };
    int result_2 = 0;

    // Defender win, capture all pieces
    std::vector<Move> moves_played_3 = {encodeMove(Attacker, SQD1, SQD4), encodeMove(Defender, SQC5, SQC4),
        encodeMove(Attacker, SQF1, SQF4), encodeMove(Defender, SQG5, SQG4), encodeMove(Attacker, SQA6, SQD6),
        encodeMove(Defender, SQC4, SQC6), encodeMove(Attacker, SQI6, SQF6), encodeMove(Defender, SQG4, SQG6),
        encodeMove(Attacker, SQB5, SQB3), encodeMove(Defender, SQD5, SQD1), encodeMove(Attacker, SQA4, SQD4),
        encodeMove(Defender, SQF5, SQF1), encodeMove(Attacker, SQH5, SQF5), encodeMove(Defender, SQG6, SQG5),
        encodeMove(Attacker, SQD9, SQD5), encodeMove(Defender, SQC6, SQC5), encodeMove(Attacker, SQD4, SQD5),
        encodeMove(King, SQE5, SQF5), encodeMove(Attacker, SQE2, SQF2), encodeMove(King, SQF5, SQF3),
        encodeMove(Attacker, SQI4, SQF4), encodeMove(Defender, SQG5, SQG4), encodeMove(Attacker, SQB3, SQD3),
        encodeMove(Defender, SQC5, SQC3), encodeMove(Attacker, SQF9, SQF7), encodeMove(Defender, SQG4, SQG7),
        encodeMove(Attacker, SQI5, SQH5), encodeMove(King, SQF3, SQF5), encodeMove(Attacker, SQH5, SQI5),
        encodeMove(Defender, SQC3, SQC5), encodeMove(Attacker, SQE9, SQF9), encodeMove(King, SQF5, SQE5),
        encodeMove(Attacker, SQI5, SQF5), encodeMove(Defender, SQG7, SQG5), encodeMove(Attacker, SQF9, SQF5),
        encodeMove(Defender, SQF1, SQF4), encodeMove(Attacker, SQA5, SQA6), encodeMove(Defender, SQE6, SQF6),
        encodeMove(Attacker, SQA6, SQD6), encodeMove(Defender, SQC5, SQC6), encodeMove(Attacker, SQE8, SQD8),
        encodeMove(Defender, SQE7, SQE8), encodeMove(Attacker, SQD8, SQD7), encodeMove(King, SQE5, SQE6),
        encodeMove(Attacker, SQD7, SQD8), encodeMove(Defender, SQC6, SQC8)
        };
    int result_3 = 0;

    // Defender win, no possible Attacker moves
    std::vector<Move> moves_played_4 = {encodeMove(Attacker, SQA4, SQA1), encodeMove(Defender, SQE3, SQF3),
        encodeMove(Attacker, SQA5, SQA2), encodeMove(Defender, SQF3, SQE3), encodeMove(Attacker, SQB5, SQB1),
        encodeMove(Defender, SQE3, SQF3), encodeMove(Attacker, SQD1, SQC1), encodeMove(Defender, SQF3, SQE3),
        encodeMove(Attacker, SQE1, SQD1), encodeMove(Defender, SQE3, SQF3), encodeMove(Attacker, SQE2, SQB2),
        encodeMove(Defender, SQF3, SQE3), encodeMove(Attacker, SQF1, SQF2), encodeMove(Defender, SQE3, SQF3),
        encodeMove(Attacker, SQF2, SQC2), encodeMove(Defender, SQF3, SQE3), encodeMove(Attacker, SQA6, SQA3),
        encodeMove(Defender, SQE3, SQE1), encodeMove(Attacker, SQI4, SQI2), encodeMove(Defender, SQE4, SQE3),
        encodeMove(Attacker, SQI2, SQD2), encodeMove(King, SQE5, SQE4), encodeMove(Attacker, SQD9, SQA9),
        encodeMove(Defender, SQE3, SQE2), encodeMove(Attacker, SQA9, SQA4), encodeMove(Defender, SQF5, SQF6),
        encodeMove(Attacker, SQH5, SQH3), encodeMove(Defender, SQF6, SQF5), encodeMove(Attacker, SQH3, SQB3),
        encodeMove(Defender, SQF5, SQF4), encodeMove(Attacker, SQI5, SQI3), encodeMove(Defender, SQF4, SQF5),
        encodeMove(Attacker, SQI3, SQC3), encodeMove(Defender, SQF5, SQF4), encodeMove(Attacker, SQI6, SQI3),
        encodeMove(Defender, SQF4, SQF5), encodeMove(Attacker, SQI3, SQD3), encodeMove(King, SQE4, SQE3),
        encodeMove(Attacker, SQE9, SQB9), encodeMove(Defender, SQF5, SQF3), encodeMove(Attacker, SQB9, SQB4),
        encodeMove(Defender, SQC5, SQA5), encodeMove(Attacker, SQE8, SQC8), encodeMove(Defender, SQD5, SQB5),
        encodeMove(Attacker, SQC8, SQC4), encodeMove(Defender, SQE7, SQC7), encodeMove(Attacker, SQF9, SQD9),
        encodeMove(Defender, SQC7, SQC5), encodeMove(Attacker, SQD9, SQD4), encodeMove(Defender, SQG5, SQD5),
        encodeMove(Attacker, SQD4, SQE4), encodeMove(Defender, SQF3, SQF4), encodeMove(Attacker, SQE4, SQD4),
        encodeMove(Defender, SQF4, SQE4)
        };
    int result_4 = 0;

    // Attacker win, king capture throne
    std::vector<Move> moves_played_5 = {encodeMove(Attacker, SQB5, SQB1), encodeMove(Defender, SQD5, SQD2),
        encodeMove(Attacker, SQD9, SQD5), encodeMove(Defender, SQE4, SQH4), encodeMove(Attacker, SQA4, SQE4),
        encodeMove(Defender, SQF5, SQF8), encodeMove(Attacker, SQF1, SQF5), encodeMove(Defender, SQE6, SQB6),
        encodeMove(Attacker, SQI6, SQE6)
        };
    int result_5 = 1;

    // Attacker win, king capture next to throne
    std::vector<Move> moves_played_6 = {encodeMove(Attacker, SQB5, SQB1), encodeMove(Defender, SQC5, SQC1),
        encodeMove(Attacker, SQA5, SQC5), encodeMove(Defender, SQD5, SQD2), encodeMove(Attacker, SQD9, SQD6),
        encodeMove(King, SQE5, SQD5), encodeMove(Attacker, SQA4, SQD4)
        };
    int result_6 = 1;

    // Attacker win, king capture on rest of board
    std::vector<Move> moves_played_7 = {encodeMove(Attacker, SQB5, SQB1), encodeMove(Defender, SQC5, SQC2), encodeMove(Attacker, SQB1, SQB3),
        encodeMove(Defender, SQD5, SQD2), encodeMove(Attacker, SQB3, SQC3), encodeMove(King, SQE5, SQB5), encodeMove(Attacker, SQC3, SQC5)
        };
    int result_7 = 1;

    // Attacker win, simultaneous king capture and piece capture
    std::vector<Move> moves_played_8 = {encodeMove(Attacker, SQI4, SQI3), encodeMove(Defender, SQE4, SQB4), encodeMove(Attacker, SQA6, SQA9),
        encodeMove(Defender, SQB4, SQB2), encodeMove(Attacker, SQD1, SQD4), encodeMove(Defender, SQE3, SQA3), encodeMove(Attacker, SQF1, SQF4),
        encodeMove(King, SQE5, SQE4), encodeMove(Attacker, SQE2, SQI2), encodeMove(Defender, SQB2, SQE2), encodeMove(Attacker, SQI3, SQE3)
        };
    int result_8 = 1;

    //Attacker win, encircled
    std::vector<Move> moves_played_9 = {encodeMove(Attacker, SQA4, SQB4), encodeMove(Defender, SQF5, SQF4), encodeMove(Attacker, SQD1, SQC1),
        encodeMove(Defender, SQF4, SQF5), encodeMove(Attacker, SQC1, SQC3), encodeMove(Defender, SQF5, SQF4), encodeMove(Attacker, SQE1, SQD1),
        encodeMove(Defender, SQF4, SQF5), encodeMove(Attacker, SQD1, SQD2), encodeMove(Defender, SQF5, SQF4), encodeMove(Attacker, SQA6, SQC6),
        encodeMove(Defender, SQF4, SQF5), encodeMove(Attacker, SQD9, SQD7), encodeMove(Defender, SQF5, SQF4), encodeMove(Attacker, SQF9, SQG9),
        encodeMove(Defender, SQF4, SQF5), encodeMove(Attacker, SQG9, SQG7), encodeMove(Defender, SQF5, SQF4), encodeMove(Attacker, SQE9, SQF9),
        encodeMove(Defender, SQF4, SQF5), encodeMove(Attacker, SQF9, SQF8), encodeMove(Defender, SQF5, SQF4), encodeMove(Attacker, SQH5, SQH6),
        encodeMove(Defender, SQF4, SQF5), encodeMove(Attacker, SQF1, SQF3), encodeMove(Defender, SQF5, SQF4), encodeMove(Attacker, SQI4, SQG4),
        encodeMove(Defender, SQF4, SQF5), encodeMove(Attacker, SQI5, SQH5)
        };
    int result_9 = 1;

    //Attacker win, encircled 2 (multiple nodes)

    // Attacker win, repetitions (immediate)
    std::vector<Move> moves_played_10 = {encodeMove(Attacker, SQB5, SQB1), encodeMove(Defender, SQC5, SQC2), encodeMove(Attacker, SQB1, SQB5),
        encodeMove(Defender, SQC2, SQC5), encodeMove(Attacker, SQB5, SQB1), encodeMove(Defender, SQC5, SQC2), encodeMove(Attacker, SQB1, SQB5),
        encodeMove(Defender, SQC2, SQC5), encodeMove(Attacker, SQB5, SQB1), encodeMove(Defender, SQC5, SQC2), encodeMove(Attacker, SQB1, SQB5),
        encodeMove(Defender, SQC2, SQC5), encodeMove(Attacker, SQB5, SQB1)
        };
    int result_10 = 1;

    // Attacker win, repetitions (after captures)
    std::vector<Move> moves_played_11 = {encodeMove(Attacker, SQD1, SQD4), encodeMove(Defender, SQC5, SQC4),
        encodeMove(Attacker, SQF1, SQF4), encodeMove(Defender, SQG5, SQG4), encodeMove(Attacker, SQA6, SQD6),
        encodeMove(Defender, SQC4, SQC6), encodeMove(Attacker, SQI6, SQF6), encodeMove(Defender, SQG4, SQG6),
        encodeMove(Attacker, SQB5, SQB3), encodeMove(Defender, SQD5, SQD1), encodeMove(Attacker, SQA4, SQD4),
        encodeMove(Defender, SQF5, SQF1), encodeMove(Attacker, SQH5, SQF5), encodeMove(Defender, SQG6, SQG5),
        encodeMove(Attacker, SQD9, SQD5), encodeMove(Defender, SQC6, SQC5), encodeMove(Attacker, SQD4, SQD5),
        encodeMove(King, SQE5, SQF5), encodeMove(Attacker, SQE2, SQF2), encodeMove(King, SQF5, SQF3),
        encodeMove(Attacker, SQI4, SQF4), encodeMove(Defender, SQG5, SQG4), encodeMove(Attacker, SQB3, SQD3),
        encodeMove(Defender, SQC5, SQC3), encodeMove(Attacker, SQF9, SQF7), encodeMove(Defender, SQG4, SQG7),
        encodeMove(Attacker, SQI5, SQH5), encodeMove(King, SQF3, SQF5), encodeMove(Attacker, SQH5, SQI5),
        encodeMove(Defender, SQC3, SQC5), encodeMove(Attacker, SQE9, SQF9), encodeMove(King, SQF5, SQE5),
        encodeMove(Attacker, SQI5, SQF5), encodeMove(Defender, SQG7, SQG5), encodeMove(Attacker, SQF9, SQF5),
        encodeMove(Defender, SQF1, SQF4), encodeMove(Attacker, SQA5, SQA6), encodeMove(Defender, SQE6, SQF6),
        encodeMove(Attacker, SQA6, SQD6), encodeMove(Defender, SQC5, SQC6), encodeMove(Attacker, SQE8, SQD8),
        encodeMove(Defender, SQE7, SQE8), encodeMove(Attacker, SQD8, SQD7), encodeMove(King, SQE5, SQE6),
        encodeMove(Attacker, SQD7, SQD8), encodeMove(Defender, SQC6, SQC7), encodeMove(Attacker, SQD8, SQD7),
        encodeMove(Defender, SQC7, SQC6), encodeMove(Attacker, SQD7, SQD8), encodeMove(Defender, SQC6, SQC7),
        encodeMove(Attacker, SQD8, SQD7), encodeMove(Defender, SQC7, SQC6), encodeMove(Attacker, SQD7, SQD8),
        encodeMove(Defender, SQC6, SQC7), encodeMove(Attacker, SQD8, SQD7), encodeMove(Defender, SQC7, SQC6),
        encodeMove(Attacker, SQD7, SQD8)
        };
    int result_11 = 1;

    init_table();
    Position::initZobrist();

    int result = play_one_game(moves_played_1);
    REQUIRE(result == result_1);
    result = play_one_game(moves_played_2);
    REQUIRE(result == result_2);
    result = play_one_game(moves_played_3);
    REQUIRE(result == result_3);
    result = play_one_game(moves_played_4);
    REQUIRE(result == result_4);
    result = play_one_game(moves_played_5);
    REQUIRE(result == result_5);
    result = play_one_game(moves_played_6);
    REQUIRE(result == result_6);
    result = play_one_game(moves_played_7);
    REQUIRE(result == result_7);
    result = play_one_game(moves_played_8);
    REQUIRE(result == result_8);
    result = play_one_game(moves_played_10);
    REQUIRE(result == result_10);
    result = play_one_game(moves_played_11);
    REQUIRE(result == result_11);
}
