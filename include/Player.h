#pragma once
#include <vector>

class Piece;
class Board;

class Player {
public:
    enum Color {
        WHITE = 1,
        BLACK = 2
    };

    Player();
    ~Player();

    void resetPlayer();
    void setColor(Color);
    int isCheck(Board*);

private:
    std::vector<Piece*> capture;
    Color color;
    bool kMove;
    bool rhMove;
    bool raMove;

    friend class Game;
};