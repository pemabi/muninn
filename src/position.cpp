#include "position.hpp"

#include <sstream>
#include <queue>

const char* Position::StartFEN = "3AAA3/4A4/4D4/A3D3A/AADDKDDAA/A3D3A/4D4/4A4/3AAA3 a 0";

namespace Zobrist {
    ZobristKey psq[PieceNum][SquareNum];
    ZobristKey repetitions[3];
    ZobristKey side;
};

// Generate random 64-bit integers for each piece-square pair, side, repetition counter
void Position::initZobrist() {
    RandomGenerator rng;

    for (PieceType pt = Defender; pt < PieceNum; ++pt) {
        for (Square sq = SQA1; sq < SquareNum; ++sq) {
            Zobrist::psq[pt][sq] = rng.random_u64();
        }
    }

    Zobrist::side = rng.random_u64();

    for (int i = 0; i < 3; ++i) {
        Zobrist::repetitions[i] = rng.random_u64();
    }
};

Position& Position::set(const std::string& fenStr, StateInfo* si) {
  /*

      Sets Position object based on modified FEN.

      FEN anatomy:
      1) Split into ranks divided by '/' character
      2) A represents attackers, D represents defenders, K represents king.
      3) use run-length for empties where number of sequential empties in a row are represented numerically.
      4) blankspace, then a or d represents Attacker or Defender turn.

      Starting FEN: "3AAA3/4A4/4D4/A3D3A/AADDKDDAA/A3D3A/4D4/4A4/3AAA3 a 1"

  */
  unsigned char token;
  Square sq = SQA9;
  std::istringstream ss(fenStr);

  std::memset(this, 0, sizeof(Position));
  std::memset(si, 0, sizeof(StateInfo));
  state = si;

  ss >> std::noskipws;

  clear();

  while ((ss >> token) && !isspace(token)) {
      if (isdigit(token)) {  // empties
          sq += (token - '0') * DeltaE;
      }

      else if (token == '/') {  // new rank
          sq += 2 * DeltaS; // typewriter reset to beginning of rank below
      }

      else if (token == 'A') {
          put_piece(Attacker, sq);
          ++sq;
      }

      else if (token == 'D') {
          put_piece(Defender, sq);
          ++sq;
      }

      else if (token == 'K') {
          put_piece(King, sq);  // put_piece will take care of updating kingIndex
          ++sq;
      }

      else {
          throw std::invalid_argument("Invalid character in FEN string " + fenStr + ": " + std::string(1, token));
      }
  }

  occupiedBB = occupied_from_pieces();
  allDefendersBB = all_defenders_from_pieces();
  win = sideNum;

  ss >> token;
  sideToMove = (token == 'a' ? Attackers : Defenders);
  ss >> std::skipws;
  int ply;
  ss >> ply;
  gamePly = ply;
  state->pliesFromNull = ply;

  set_state(state);
  std::cout<<"inital key: "<<state->key<<'\n';

  return *this;
}

void Position::clear() {
    attackerBB = all_zero_bb();
    defenderBB = all_zero_bb();
    kingBB = all_zero_bb();
    allDefendersBB = all_zero_bb();
    occupiedBB = all_zero_bb();

    kingIndex = SQ_NONE;
    gamePly = 0;
    sideToMove = Attackers;
}

// takes king index and returns an index for the attacker configuration necessary for king capture. Only called when certain that the king index is one of these 5 sqs
int compress_king_index(int idx) {
    switch(idx) {
        case 31: return 0;
        case 39: return 1;
        case 40: return 2;
        case 41: return 3;
        case 49: return 4;
        default: return -1; // Error case
    }
}

Bitboard Position::make_captures(Square to) {
    Bitboard attacked_squares, captures, ally_key;
    if (sideToMove == Defenders) {
        ally_key = allDefendersBB | THRONE_MASK;  // OR with throne mask because empty or full throne can act as an ally for capturing purposes
        attacked_squares = get_attacked_squares(to, ally_key);
        captures = attackerBB & attacked_squares;
        attackerBB ^= captures;
        occupiedBB ^= captures;
    }
    else {
        if (kingIndex != 40) {  // check if king is on the throne
            ally_key = attackerBB | THRONE_MASK;  // OR with throne mask if throne is empty and can act as an ally for capturing purposes
        }
        else {
            ally_key = attackerBB;
        }
        attacked_squares = get_attacked_squares(to, ally_key);
        captures = defenderBB & attacked_squares;  // to store which pieces have been captured.
        defenderBB ^= captures;
        if (attacked_squares & kingBB) {  // If overlap on King BB and attacked sqs, then make some further checks. Worry is potential for unnoticed floating bits outside of board area
            if (AROUND_THRONE_MASK & kingBB) {  // check if King is on a square with special capture requirements (on / adjacent to throne)
                Bitboard attack_pattern = king_attack_pattern[compress_king_index(kingIndex)];
                if (attack_pattern == (attack_pattern & ally_key)) {
                    captures ^= kingBB;  // add the king capture to captures so it is XORed out of allDefendersBB and occupied at end of sequence
                    kingBB ^= SquareMaskBB[kingIndex];
                }
            }
            else {
                captures ^= kingBB;
                kingBB ^= SquareMaskBB[kingIndex];
            }
        }
        allDefendersBB ^= captures;
        occupiedBB ^= captures;
    }
    return captures;
    // could modify attack hashing so that I don't need all of the safety checks here - remove attacks where allies are in neighbouring sq
    // should really test this vs just rebuilding the occupied and allDefenders with & operation
}

void Position::do_move(Move move, StateInfo& newState) {
    assert(is_valid_move(move));

    ZobristKey k = state->key;
    std::cout<<"key at start of move: "<<k<<'\n';
    k ^= Zobrist::side;
    std::cout<<"key updated to new side: "<<k<<'\n';

    // handle state transfer
    // offsetof() means that I'm only copying up to previous. Everything else is computed during do_move for the new StateInfo object
    // after we switch out state pointer to point to the new state.
    std::memcpy(&newState, state, offsetof(StateInfo, previous));
    newState.previous = state;
    state = &newState;

    // increment plies
    ++gamePly;
    ++state->pliesFromNull;

    Side us = sideToMove;
    Side them = ~us;
    Square from = move.from();
    Square to = move.to();
    PieceType pt = move.movedPiece();

    assert(move.movedSide() == us);

    move_piece(pt, from, to);

    // increment hash key
    k ^= Zobrist::psq[pt][from] ^ Zobrist::psq[pt][to];
    std::cout<<"key updated post move: "<<k<<'\n';

    Bitboard captures = make_captures(to); // make captures on position (including King), returning captured pawns (NOT KING) and saving them in StateInfo object
    state->capturesBB = captures;

    // loop thru captures to update Zobrist key
    PieceType captured_pt = (us == Defenders) ? Attacker : Defender;
    while (captures) {
        Square sq = captures.bitscan_pop_forward();
        k ^= Zobrist::psq[captured_pt][sq];
        std::cout<<"key updated post capture: "<<k<<'\n';
    }

    // If king has been captured during captures routine, updates zobrist key, sets index to invalid square, and sets win
    if (kingBB == EMPTY_BB) {
        k ^= Zobrist::psq[King][kingIndex];
        std::cout<<"key updated post king capture: "<<k<<'\n';
        kingIndex = SquareNum;
        win = Attackers;
    }

    state->key = k;

    sideToMove = ~sideToMove;

    state->move = move;
}

// checks if position is surrounded
bool Position::is_surrounded(const Bitboard& allToSquares) const {
    std::queue<Square> queue;

    Bitboard reachable_sqs = allToSquares | all_defenders_bb();
    Bitboard visited = reachable_sqs;

    // if defender is on / can move to the edge of the board
    if (reachable_sqs & EDGE_MASK) {
        return false;
    }

    while (reachable_sqs) {
        Square sq = reachable_sqs.bitscan_pop_forward();
        queue.push(sq);
    }

    while (!queue.empty()) {
        Square current = queue.front();
        queue.pop();

        if (SquareMaskBB[current] & EDGE_MASK) {
            return false;
        }

        const Square* neighbour_sqs = get_adjacent_squares(current);
        for (int i = 0; i < MAX_NEIGHBOURS; i++) {
            Square next = neighbour_sqs[i];
            if (next == SQ_NONE) break;
            if (!visited.is_set(next) && !attackerBB.is_set(next)) {
                queue.push(next);
                visited.set_bit(next);
                if (visited & EDGE_MASK) {
                    return false;
                }
            }
        }
    }
    return true;
}

void print_position_bbs(const Position& pos) {
    std::cout<<"ATTACKERS"<<'\n';
    pos.attacker_bb().print_board();
    std::cout<<"DEFENDERS"<<'\n';
    pos.defender_bb().print_board();
    std::cout<<"KING\n";
    pos.king_bb().print_board();
    std::cout<<"ALL DEFENDERS\n";
    pos.all_defenders_bb().print_board();
    std::cout<<"OCCUPIED\n";
    pos.occupied_bb().print_board();
}

void print_position(const Position& pos) {
    // Unicode pieces
    const std::string DEFENDER = "♟";
    const std::string KING = "♕";  // dark theme makes the white king annoying. Using the Queen unicode character instead.
    const std::string ATTACKER = "♙";
    const std::string EMPTY = "·";

    // go through each bitboard, print piece on the square. Maybe check all piece bbs and if there are multiple pieces on the same square, generate an error
    //TODO: make a windows compatible one with 'A''D' and 'K'

    // check for overlap in bbs
    if (pos.occupied_bb() == (pos.attacker_bb() ^ pos.defender_bb() ^ pos.king_bb())) {

        for (Rank r = Rank9; r >= Rank1; --r) {
            std::cout << r+1 << " ";
            for (File f = FileA; f < FileNum; ++f) {
                Square sq = index_square(f, r);
                if (pos.attacker_bb().is_set(sq)) {
                    std::cout << ATTACKER << " ";
                }
                else if (pos.defender_bb().is_set(sq)) {
                    std::cout << DEFENDER << " ";
                }
                else if (pos.king_bb().is_set(sq)) {  //could just use King index here
                    std::cout << KING << " ";
                }
                else {
                    std::cout << EMPTY << " ";
                }
            }
            std::cout << "\n";
        }
        std::cout << "  a b c d e f g h i\n";
    }
    else {
        std::cout << "overlap in position Bitboards on Square!\n";
        print_position_bbs(pos);
    }
}

void Position::set_state(StateInfo* si) const {
    si->key = 0;

    for (Bitboard b = defender_bb(); b; ) {
        Square sq = b.bitscan_pop_forward();
        si->key ^= Zobrist::psq[Defender][sq];
    }
    for (Bitboard b = king_bb(); b; ) {
        Square sq = b.bitscan_pop_forward();
        si->key ^= Zobrist::psq[King][sq];
    }
    for (Bitboard b = attacker_bb(); b; ) {
        Square sq = b.bitscan_pop_forward();
        si->key ^= Zobrist::psq[Attacker][sq];
    }

    if (side_to_move() == Defenders) {
        si->key ^= Zobrist::side;
    }
}

// Adds on the repetitons to the Zobrist key. Keeping separate because 'raw' positional key important for repetition tracking, etc
ZobristKey Position::full_key() const {
    //auto reps = std::min(2, repetitions_count());  // max is 2 repetitions - in theory shouldnt need this except for invalid input?
    auto reps = repetitions_count();
    assert(reps <= 2);

    return state->key ^ Zobrist::repetitions[reps];
}

// iterates backwards thru the states, comparing keys
int Position::repetitions_count() const {
    int end = state->pliesFromNull;

    if (end < 4)  // minimum ply at which repetitions can start occuring
        return 0;

    StateInfo* state_p = state->previous->previous;  // points to side's last turn
    int rep_count = 0;

    for (int i = 4; i <= end; i += 2)
    {
        state_p = state_p->previous->previous;
        if (state_p->capturesBB) {  // early breaking when there are captures - impossible for positions to be repeated. Could add a flag or counter (similar to chess 50 move rule)
            return rep_count;
        }
        if (state_p->key == state->key) {
            rep_count++;
        }
    }
    return rep_count;
}

void BoardHistory::set(const std::string& fen) {
    positions.clear();
    states.clear();

    positions.emplace_back();
    states.emplace_back(new StateInfo());
    current_pos().set(fen, states.back().get());
}

BoardHistory BoardHistory::shallow_clone() const {
    BoardHistory bh;
    for (int i = std::max(0, static_cast<int>(positions.size()) - 8); i < static_cast<int>(positions.size()); ++i) {
        bh.positions.push_back(positions[i]);
    }
    return bh;
}

void BoardHistory::do_move(Move m) {
    states.emplace_back(new StateInfo);
    positions.push_back(positions.back());
    positions.back().do_move(m, *states.back());
}

bool BoardHistory::undo_move() {
    if (positions.size() == 1) return false;
    states.pop_back();
    positions.pop_back();
    return true;
}
