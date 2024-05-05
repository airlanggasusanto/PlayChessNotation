#include "Move.h"
#include <iostream>
#include <regex>
#include <iomanip>

Move::Move()
    : Notation(""), number(0), pieceId(0), pieceType(0), color(0),
    from(""), target(""), captureId(0), promotionType(0),
    isCheck(false), isMate(false), isCastleKing(false), isCastleQueen(false), isEnpassant(false), isPawnDoubleMoves(false), ecId(0){

}

Move::~Move() {


}

std::string Move::getNotation() {
	std::string notation;
	if (isCastleKing) {
		notation = "O-O";
		return notation;
	}
	else if (isCastleQueen) {
		notation = "O-O-O";
		return notation;
	}
	if (pieceType != 1) {
		if (pieceType == 2) {
			notation += "R";
		}
		else if (pieceType == 3) {
			notation += "N";
		}
		else if (pieceType == 4) {
			notation += "B";
		}
		else if (pieceType == 5) {
			notation += "Q";
		}
		else if (pieceType == 6) {
			notation += "K";
		}
	}

	from = "";
	parseFrom();

	if (!from.empty()) {
		notation += from;
	}

	if (captureId) {
		notation += 'x';
	}

	// Add target square
	notation += target;

	//if (isEnpassant) {
	//	notation += " (e.p)";
	//}

	// Add promotion piece type (if applicable)
	if (promotionType) {
		notation += "=";
		if (promotionType == 2) {
			notation += "R";
		}
		else if (promotionType == 3) {
			notation += "N";
		}
		else if (promotionType == 4) {
			notation += "B";
		}
		else if (promotionType == 5) {
			notation += "Q";
		}
		else if (promotionType == 6) {
			notation += "K";
		}
	}

	// Add check or mate symbols
	if (isMate) {
		notation += '#';
	}
	else if (isCheck) {
		notation += '+';
	}

	return notation;
}

void Move::printData() const {
	std::cout << "Notation: " << Notation << std::endl;
	std::cout << "Number: " << number << std::endl;
	std::cout << "Piece ID: " << pieceId << std::endl;
	std::cout << "Piece Type: " << pieceType << std::endl;
	std::cout << "Color: " << color << std::endl;
	std::cout << "From: " << from << std::endl;
	std::cout << "Target: " << target << std::endl;
	std::cout << "Capture ID: " << captureId << std::endl;
	std::cout << "Promotion Type: " << promotionType << std::endl;
	std::cout << "Is Check: " << isCheck << std::endl;
	std::cout << "Is Mate: " << isMate << std::endl;
	std::cout << "Is Castle King: " << isCastleKing << std::endl;
	std::cout << "Is Castle Queen: " << isCastleQueen << std::endl;
}


void Move::parseFrom() {
	std::regex moveRegex("([KQRBN]?)([a-h]?)([1-8]?)([x]?)([a-h][1-8]|O-O(-O)?)(=[QRBN])?([+#])?");
	std::smatch match;

	if (!std::regex_match(Notation, match, moveRegex)) {
		return;
	}

	from = match[2].str() + match[3].str();
}

bool Move::inputMove(std::string m) {
	std::regex moveRegex("([KQRBN]?)([a-h]?)([1-8]?)([x]?)([a-h][1-8]|O-O(-O)?)(=[QRBN])?([+#])?");
	std::smatch match;

	if (!std::regex_match(m, match, moveRegex)) {
		std::cout << "Invalid move notation: " << m << std::endl;
		return false;
	}

	if (match.str() == "O-O") {
		Notation = "O-O";
		isCastleKing = true;
		return true;
	}
	else if (match.str() == "O-O-O") {
		Notation = "O-O-O";
		isCastleQueen = true;
		return true;
	}

	Notation = match.str();

	std::string pt = match[1].str();
	target = match[5].str();

	if (target.empty()) {
		return false;
	}

	if (pt == "Q") {
		pieceType = 5;
	}
	else if (pt == "K") {
		pieceType = 6;
	}
	else if (pt == "N") {
		pieceType = 3;
	}
	else if (pt == "R") {
		pieceType = 2;
	}
	else if (pt == "B") {
		pieceType = 4;
	}
	else if (pt == "") {
		pieceType = 1;
	}
	std::string promotion = match[7].str();
	if (!promotion.empty()) {
		promotion = promotion.substr(1);
	}

	if (promotion == "Q") {
		promotionType = 5;
	}
	else if (promotion == "K") {
		promotionType = 6;
	}
	else if (promotion == "N") {
		promotionType = 3;
	}
	else if (promotion == "R") {
		promotionType = 2;
	}
	else if (promotion == "B") {
		promotionType = 4;
	}
	else if (promotion == "") {
		promotionType = 0;
	}

	return true;
}