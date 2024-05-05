#pragma once
#include<string>

class Player;
class Board;

class Piece {
public:
    enum Type {
        PAWN = 1,
        ROOK = 2,
        KNIGHT = 3,
        BISHOP = 4,
        QUEEN = 5,
        KING = 6
    };

    Piece();
    Piece(Type, Player::Color);
    ~Piece();

    void setPiece(Type, Player::Color);
    void setId(std::string);
    int isPinnedDiagonal(Board*);
    int isPinnedRank(Board*);
    int isPinnedFile(Board*);

private:
    Type type;
    Player::Color color;
    int id;

    friend class Player;
    friend class Board;
    friend class Game;
};