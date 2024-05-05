#pragma once
#include <map>
#include <string>

class Player;
class Piece;

class Board {
public:
	Board();
	~Board();

	void clearBoard();
	void setBoard(Player&, Player&);
	void printBoard();

	bool canAttackSquare(std::string, std::string);

	void customPos1();
	void customPos2();
	void customPos3();
	void customPos4();
	void customPos5();
	void customPos6();
	void customPos7();
	void customPos8();
	void customPos9();
	void customPos10();
	void customPos11();
	void customPos12();

private:
	std::map<std::string, Piece*> board;

	friend class Player;
	friend class Piece;
	friend class Game;
};