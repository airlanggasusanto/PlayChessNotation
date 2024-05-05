#pragma once
#include "Player.h"
#include "Piece.h"
#include "Board.h"
#include "Bot.h"
#include "Move.h"
#include <vector>


class Game {
public:
	Game();
	~Game();

	bool getPawnPos(Move& m);

	bool getPos(Move& m);

	bool kingMove(Move& m);

	bool pawnMove(Move& m);

	bool pieceMove(Move& m);

	//bool moveInCheck(int enemyId, Move m);

	//bool moveInNormal(Move m);

	bool makeMove(Move& m);

	void gameMenu();

	bool isGameOver();

	void displayNotation();

	void saveNotation();

	void displayResult();

	void vsBot();

	void vs2Player();

	void start();

private:
	Board board;
	Player white;
	Player black;
	std::vector<Move> record;
};