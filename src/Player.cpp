#include "Player.h"
#include "Piece.h"
#include "Board.h"
#include <algorithm>

Player::Player() {

}

Player::~Player() {

}

void Player::resetPlayer() {
	kMove = false;
	rhMove = false;
	raMove = false;
}

void Player::setColor(Color c) {
	color = c;
}

int Player::isCheck(Board* b) {
    std::string kPos = "";
    // determine king position
    for (const auto& entry : b->board) {
        Piece* piece = entry.second;
        if (piece != nullptr && piece->type == Piece::KING && piece->color == color) {
            kPos = entry.first;
            break;
        }
    }

    Player::Color enemy;
    enemy = (color == WHITE) ? BLACK : WHITE;

    // saving the king 
    Piece* temp = nullptr;
    std::swap(b->board[kPos], temp);
    for (const auto& entry : b->board) {
        Piece* piece = entry.second;
        std::string ppos = entry.first;
        if (piece != nullptr && piece->color == enemy) {
            if (b->canAttackSquare(entry.first, kPos)) {
                // send it back again
                std::swap(b->board[kPos], temp);
                return entry.second->id;
            }
        }
    }
    // send it back again
    std::swap(b->board[kPos], temp);

    return 0;
}