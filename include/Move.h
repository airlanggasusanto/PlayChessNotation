#pragma once
#include <string>

class Board;
class Piece;
class Player;

class Move {
public:
    Move();
    ~Move();

    void parseFrom();
    bool inputMove(std::string m);
    void printData() const;
    std::string getNotation();

private:
    std::string Notation;
    int number;
    int pieceId;
    int pieceType;
    int color;
    std::string from;
    std::string target;
    int captureId;
    int ecId;
    int promotionType;
    bool isEnpassant;
    bool isPawnDoubleMoves;
    bool isCheck;
    bool isMate;
    bool isCastleKing;
    bool isCastleQueen;

    friend class Game;
    friend class Board;
};
