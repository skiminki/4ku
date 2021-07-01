#include "attack.hpp"
#include "bitboard.hpp"
#include "position.hpp"
#include "raycast.hpp"

namespace chess {

[[nodiscard]] bool attacked(const Position &pos, const int sq, const bool them) {
    const auto bb = Bitboard(sq);
    const auto kt = pos.colour[them] & pos.pieces[static_cast<int>(Piece::Knight)];
    const auto BQ = pos.pieces[static_cast<int>(Piece::Bishop)] | pos.pieces[static_cast<int>(Piece::Queen)];
    const auto RQ = pos.pieces[static_cast<int>(Piece::Rook)] | pos.pieces[static_cast<int>(Piece::Queen)];
    const auto pawns = pos.colour[them] & pos.pieces[static_cast<int>(Piece::Pawn)];
    const auto pawn_attacks =
        them ? pawns.south().west() | pawns.south().east() : pawns.north().west() | pawns.north().east();

    // Pawns
    if (pawn_attacks & bb) {
        return true;
    }

    // Knights
    if (bb &
        (kt.north().north().west() | kt.north().north().east() | kt.south().south().west() | kt.south().south().east() |
         kt.east().east().north() | kt.east().east().south() | kt.west().west().north() | kt.west().west().south())) {
        return true;
    }

    // Bishop & Queen
    if (raycast::bishop(sq, pos.colour[0] | pos.colour[1]) & pos.colour[them] & BQ) {
        return true;
    }

    // Rook & Queen
    if (raycast::rook(sq, pos.colour[0] | pos.colour[1]) & pos.colour[them] & RQ) {
        return true;
    }

    // King
    if (bb.adjacent() & pos.colour[them] & pos.pieces[static_cast<int>(Piece::King)]) {
        return true;
    }

    return false;
}

}  // namespace chess