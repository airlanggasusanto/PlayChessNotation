#include "Player.h"
#include "Piece.h"
#include "Board.h"

Piece::Piece() {

}

Piece::~Piece() {

}

int squareToInt(const std::string& square) {
    int result = 0;
    for (char c : square) {
        result = result * 256 + static_cast<int>(c); // Combine ASCII values
    }
    return result;
}

Piece::Piece(Type t, Player::Color c) {
	type = t;
	color = c;
    id = 0;
}

void Piece::setPiece(Type t, Player::Color c) {
	type = t;
	color = c;
}

void Piece::setId(std::string square) {
    id = squareToInt(square);
}

bool areOnSameDiagonal(const std::string& pos1, const std::string& pos2, const std::string& pos3) {
    // Calculate absolute differences in ranks and files between positions
    int rankDiff1 = pos1[1] - '0';
    int fileDiff1 = pos1[0] - 'a';

    int rankDiff2 = pos2[1] - '0';
    int fileDiff2 = pos2[0] - 'a';

    int rankDiff3 = pos3[1] - '0';
    int fileDiff3 = pos3[0] - 'a';

    // Check if absolute differences are equal
    return ((rankDiff1 - rankDiff2 == fileDiff1 - fileDiff2) && (rankDiff1 - rankDiff3 == fileDiff1 - fileDiff3));
}

int Piece::isPinnedDiagonal(Board* b) {
    std::string kPos = "";
    // determine king position
    for (const auto& entry : b->board) {
        Piece* piece = entry.second;
        if (piece != nullptr && piece->type == Piece::KING && piece->color == color) {
            kPos = entry.first;
            break;
        }
    }

    std::string pPos = "";
    // determine king position
    for (const auto& entry : b->board) {
        Piece* piece = entry.second;
        if (piece != nullptr && piece->id == id) {
            pPos = entry.first;
            break;
        }
    }

    // Convert positions to row and column indices
    int kRow = kPos[1] - '1';
    int kCol = kPos[0] - 'a';
    int pRow = pPos[1] - '1';
    int pCol = pPos[0] - 'a';

    // Check if positions are on the same diagonal
    if (abs(kRow - pRow) == abs(kCol - pCol)) {
        // Positions are on the same diagonal

        for (const auto& entry : b->board) {
            if (entry.second->color == color) {
                continue;
            }

            if (entry.second->type == BISHOP || entry.second->type == QUEEN) {
                if (!areOnSameDiagonal(kPos, pPos, entry.first)) {
                    continue;
                }
                Piece* ptemp = nullptr;
                std::swap(b->board[pPos], ptemp);

                if (!b->canAttackSquare(entry.first, pPos)) {
                    std::swap(b->board[pPos], ptemp);
                    continue;
                }

                Piece* ktemp = nullptr;
                std::swap(b->board[kPos], ktemp);

                if (!b->canAttackSquare(entry.first, kPos)) {
                    std::swap(b->board[pPos], ptemp);
                    std::swap(b->board[kPos], ktemp);
                    continue;
                }

                std::swap(b->board[kPos], ktemp);
                std::swap(b->board[pPos], ptemp);
                return entry.second->id;
            }
        }
    }

    return 0;
}

bool areOnSameFile(const std::string& pos1, const std::string& pos2, const std::string& pos3) {
    return (pos1[0] == pos2[0] && pos1[0] == pos3[0]);
}

int Piece::isPinnedFile(Board* b) {
    std::string kPos = "";
    // determine king position
    for (const auto& entry : b->board) {
        Piece* piece = entry.second;
        if (piece != nullptr && piece->type == Piece::KING && piece->color == color) {
            kPos = entry.first;
            break;
        }
    }

    std::string pPos = "";
    // determine piece position
    for (const auto& entry : b->board) {
        Piece* piece = entry.second;
        if (piece != nullptr && piece->id == id) {
            pPos = entry.first;
            break;
        }
    }

    if (pPos[0] != kPos[0]) {
        // check if not the same file
        return 0;
    }


    for (const auto& entry : b->board) {
        if (entry.second->color == color) {
            continue;
        }

        if (entry.second->type == ROOK || entry.second->type == QUEEN) {
            std::string tPos = entry.first;
            if (!areOnSameFile(kPos, pPos, tPos)) {
                continue;
            }
            Piece* ptemp = nullptr;
            std::swap(ptemp, b->board[pPos]);

            if (!b->canAttackSquare(tPos, pPos)) {
                std::swap(ptemp, b->board[pPos]);
                continue;
            }

            Piece* ktemp = nullptr;
            std::swap(ktemp, b->board[kPos]);

            if (!b->canAttackSquare(tPos, kPos)) {
                std::swap(ktemp, b->board[kPos]);
                std::swap(ptemp, b->board[pPos]);
                continue;
            }

            std::swap(ktemp, b->board[kPos]);
            std::swap(ptemp, b->board[pPos]);
            return entry.second->id;
        }
    }

    return 0;
}

bool areOnSameRank(const std::string& pos1, const std::string& pos2, const std::string& pos3) {
    return (pos1[1] == pos2[1] && pos1[1] == pos3[1]);
}

int Piece::isPinnedRank(Board* b) {
    std::string kPos = "";
    // determine king position
    for (const auto& entry : b->board) {
        Piece* piece = entry.second;
        if (piece != nullptr && piece->type == Piece::KING && piece->color == color) {
            kPos = entry.first;
            break;
        }
    }

    std::string pPos = "";
    // determine piece position
    for (const auto& entry : b->board) {
        Piece* piece = entry.second;
        if (piece != nullptr && piece->id == id) {
            pPos = entry.first;
            break;
        }
    }

    // check if the same rank
    if (kPos[1] != pPos[1]) {
        return 0;
    }

    for (const auto& entry : b->board) {
        if (entry.second->color == color) {
            continue;
        }

        if (entry.second->type == ROOK || entry.second->type == QUEEN) {
            std::string tPos = entry.first;
            if (!areOnSameRank(kPos, pPos, tPos)) {
                continue;
            }

            Piece* ptemp = nullptr;
            std::swap(ptemp, b->board[pPos]);

            if (!b->canAttackSquare(tPos, pPos)) {
                std::swap(ptemp, b->board[pPos]);
                continue;
            }

            Piece* ktemp = nullptr;
            std::swap(ktemp, b->board[kPos]);

            if (!b->canAttackSquare(tPos, kPos)) {
                std::swap(ktemp, b->board[kPos]);
                std::swap(ptemp, b->board[pPos]);
                continue;
            }

            std::swap(ktemp, b->board[kPos]);
            std::swap(ptemp, b->board[pPos]);
            return entry.second->id;
        }
    }

    return 0;
}