#include "Player.h"
#include "Piece.h"
#include "Board.h"
#include <iostream>

Board::Board() {

}

Board::~Board() {
	clearBoard();
}

void Board::clearBoard() {
	for (auto& pair : board) {
		if (pair.second != nullptr) {
			delete pair.second;  
				pair.second = nullptr;
		}
	}
	board.clear();
}

void Board::setBoard(Player&w,Player&b) {
	clearBoard();
	w.resetPlayer();
	b.resetPlayer();
	w.setColor(Player::Color::WHITE);
	b.setColor(Player::Color::BLACK);

	for (char file = 'a'; file <= 'h'; ++file) {
		for (int rank = 1; rank <= 8; ++rank) {
			std::string position = std::string(1, file) + std::to_string(rank);
			board[position] = nullptr;
		}
	}

	// Set up pieces on the board
	board["a1"] = new Piece{ Piece::ROOK, Player::WHITE };
	board["a1"]->setId("a1");
	board["h1"] = new Piece{ Piece::ROOK, Player::WHITE };
	board["h1"]->setId("h1");
	board["a8"] = new Piece{ Piece::ROOK, Player::BLACK };
	board["a8"]->setId("a8");
	board["h8"] = new Piece{ Piece::ROOK, Player::BLACK };
	board["h8"]->setId("h8");

	board["b1"] = new Piece{ Piece::KNIGHT, Player::WHITE };
	board["b1"]->setId("b1");
	board["g1"] = new Piece{ Piece::KNIGHT, Player::WHITE };
	board["g1"]->setId("g1");
	board["b8"] = new Piece{ Piece::KNIGHT, Player::BLACK };
	board["b8"]->setId("b8");
	board["g8"] = new Piece{ Piece::KNIGHT, Player::BLACK };
	board["g8"]->setId("g8");

	board["c1"] = new Piece{ Piece::BISHOP, Player::WHITE };
	board["c1"]->setId("c1");
	board["f1"] = new Piece{ Piece::BISHOP, Player::WHITE };
	board["f1"]->setId("f1");
	board["c8"] = new Piece{ Piece::BISHOP, Player::BLACK };
	board["c8"]->setId("c8");
	board["f8"] = new Piece{ Piece::BISHOP, Player::BLACK };
	board["f8"]->setId("f8");

	board["d1"] = new Piece{ Piece::QUEEN, Player::WHITE };
	board["d1"]->setId("d1");
	board["d8"] = new Piece{ Piece::QUEEN, Player::BLACK };
	board["d8"]->setId("d8");

	board["e1"] = new Piece{ Piece::KING, Player::WHITE };
	board["e1"]->setId("e1");
	board["e8"] = new Piece{ Piece::KING, Player::BLACK };
	board["e8"]->setId("e8");

	for (char file = 'a'; file <= 'h'; ++file) {
		board[std::string(1, file) + "2"] = new Piece{ Piece::PAWN, Player::WHITE };
		board[std::string(1, file) + "2"]->setId(std::string(1, file) + "2");
		board[std::string(1, file) + "7"] = new Piece{ Piece::PAWN, Player::BLACK };
		board[std::string(1, file) + "7"]->setId(std::string(1, file) + "7");
	}

	return;
}

void Board::printBoard() {
	std::cout << "  +------------------------+" << std::endl;
	for (int rank = 8; rank >= 1; --rank) {
				std::cout << rank << " | ";
				for (char file = 'a'; file <= 'h'; ++file) {
					std::string position = std::string(1, file) + std::to_string(rank);
						if (board.count(position) && board.at(position) != nullptr) {
								switch (board.at(position)->type) {
								case Piece::PAWN:
									std::cout << "P";
									break;
								case Piece::ROOK:
									std::cout << "R";
									break;
								case Piece::KNIGHT:
									std::cout << "N";
									break;
								case Piece::BISHOP:
									std::cout << "B";
									break;
								case Piece::QUEEN:
									std::cout << "Q";
									break;
								case Piece::KING:
									std::cout << "K";
									break;
								}
								std::cout << (board.at(position)->color == Player::WHITE ? "W " : "B ");
						}
						else {
								std::cout << " - ";
						}
				}
				std::cout << std::endl;
		}
		std::cout << "  +------------------------+" << std::endl;

		std::cout << "    ";
	for (char file = 'a'; file <= 'h'; ++file) {
		std::cout << file << "  ";
	}
	std::cout << std::endl;
	return;
}

bool Board::canAttackSquare(std::string from, std::string target) {
	Piece* attackingPiece = board[from];
	if (attackingPiece == nullptr) {
		return false; // No piece at the 'from' position
	}

	if (attackingPiece->type == Piece::ROOK) {
		if (from[0] == target[0] || from[1] == target[1]) {
			if (from[0] == target[0]) {
				char startFile = std::min(from[1], target[1]) + 1;
				char endFile = std::max(from[1], target[1]) - 1;
				for (char file = startFile; file <= endFile; ++file) {
					std::string square = std::string(1, from[0]) + file;
					if (board[square] != nullptr) {
						return false; // There's a piece blocking the path
					}
				}
				return true; // No piece blocking the path
			}
			else {
				int startRank = std::min(from[0], target[0]) + 1 - '0';
				int endRank = std::max(from[0], target[0]) - 1 - '0';
				for (int rank = startRank; rank <= endRank; ++rank) {
					std::string square = std::string(1, rank + '0') + from[1];
					if (board[square] != nullptr) {
						return false; // There's a piece blocking the path
					}
				}
				return true; // No piece blocking the path
			}
		}
		return false; // Target square is not in the same row or column
	}
	else if (attackingPiece->type == Piece::KNIGHT) {
		int fileDiff = abs(from[0] - target[0]);
		int rankDiff = abs(from[1] - target[1]);

		if ((fileDiff == 2 && rankDiff == 1) || (fileDiff == 1 && rankDiff == 2)) {
			return true; // Knight can attack the target square
		}
		else {
			return false; // Target square is not a valid move position for the knight
		}
	}
	else if (attackingPiece->type == Piece::BISHOP) {
		int fileDiff = abs(from[0] - target[0]);
		int rankDiff = abs(from[1] - target[1]);
		if (fileDiff != rankDiff) {
			return false; // Target square is not on the same diagonal
		}

		int fileDir = (target[0] > from[0]) ? 1 : -1;
		int rankDir = (target[1] > from[1]) ? 1 : -1;

		char file = from[0] + fileDir;
		char rank = from[1] + rankDir;
		while (file != target[0] && rank != target[1]) {
			std::string square = std::string(1, file) + std::string(1, rank);
			if (board[square] != nullptr) {
				return false; // There's a piece blocking the path
			}
			file += fileDir;
			rank += rankDir;
		}

		return true;
	}
	else if (attackingPiece->type == Piece::QUEEN) {
		int fileDiff = abs(from[0] - target[0]);
		int rankDiff = abs(from[1] - target[1]);

		if (from[0] == target[0] || from[1] == target[1] || fileDiff == rankDiff) {
			if (from[0] == target[0]) {
				char startFile = std::min(from[1], target[1]) + 1;
				char endFile = std::max(from[1], target[1]) - 1;
				for (char file = startFile; file <= endFile; ++file) {
					std::string square = std::string(1, from[0]) + file;
					if (board[square] != nullptr) {
						return false; // There's a piece blocking the path
					}
				}
				return true; // No piece blocking the path
			}
			else if (from[1] == target[1]) {
				int startRank = std::min(from[0], target[0]) + 1 - '0';
				int endRank = std::max(from[0], target[0]) - 1 - '0';
				for (int rank = startRank; rank <= endRank; ++rank) {
					std::string square = std::string(1, rank + '0') + from[1];
					if (board[square] != nullptr) {
						return false; // There's a piece blocking the path
					}
				}
				return true; // No piece blocking the path
			}
			else {
				int fileDir = (target[0] > from[0]) ? 1 : -1;
				int rankDir = (target[1] > from[1]) ? 1 : -1;
				char file = from[0] + fileDir;
				char rank = from[1] + rankDir;
				while (file != target[0] && rank != target[1]) {
					std::string square = std::string(1, file) + std::string(1, rank);
					if (board[square] != nullptr) {
						return false; // There's a piece blocking the path
					}
					file += fileDir;
					rank += rankDir;
				}
				return true; // No piece blocking the path
			}
		}
		return false;
	}
	else if (attackingPiece->type == Piece::KING) {
		int fileDiff = abs(from[0] - target[0]);
		int rankDiff = abs(from[1] - target[1]);

		if (fileDiff <= 1 && rankDiff <= 1) {
			return true; // King can attack the target square
		}
		else {
			return false; // Target square is not adjacent to the king's current position
		}
	}
	else if (attackingPiece->type == Piece::PAWN) {
		int direction = (attackingPiece->color == Player::WHITE) ? 1 : -1;
		int fileDiff = abs(from[0] - target[0]);
		int rankDiff = abs(from[1] - target[1]);

		if (fileDiff == 1 && rankDiff == 1 && (target[1] - from[1]) == direction) {
			return true; // Pawn can attack the target square
		}
		else {
			return false; // Target square is not diagonally adjacent to the pawn's current position in the correct direction
		}
	}
	return false;
}

// pinned pawn
void Board::customPos1() {
	clearBoard();
	for (char file = 'a'; file <= 'h'; ++file) {
		for (int rank = 1; rank <= 8; ++rank) {
			std::string position = std::string(1, file) + std::to_string(rank);
			board[position] = nullptr;
		}
	}

	board["a2"] = new Piece{ Piece::BISHOP, Player::WHITE };
	board["g1"] = new Piece{ Piece::KING, Player::WHITE };
	board["h6"] = new Piece{ Piece::QUEEN, Player::WHITE };
	board["g6"] = new Piece{ Piece::ROOK, Player::WHITE };
	board["f2"] = new Piece{ Piece::PAWN, Player::WHITE };

	board["g8"] = new Piece{ Piece::KING, Player::BLACK };
	board["f8"] = new Piece{ Piece::QUEEN, Player::BLACK };
	board["g7"] = new Piece{ Piece::PAWN, Player::BLACK };
	board["f7"] = new Piece{ Piece::PAWN, Player::BLACK };
	board["f6"] = new Piece{ Piece::BISHOP, Player::BLACK };
}

//pinned rock and queen
void Board::customPos2() {
	clearBoard();
	for (char file = 'a'; file <= 'h'; ++file) {
		for (int rank = 1; rank <= 8; ++rank) {
			std::string position = std::string(1, file) + std::to_string(rank);
			board[position] = nullptr;
		}
	}

	board["a3"] = new Piece{ Piece::PAWN, Player::WHITE };
	board["b4"] = new Piece{ Piece::PAWN, Player::WHITE };
	board["c1"] = new Piece{ Piece::ROOK, Player::WHITE };
	board["e2"] = new Piece{ Piece::QUEEN, Player::WHITE };
	board["e3"] = new Piece{ Piece::PAWN, Player::WHITE };
	board["f2"] = new Piece{ Piece::KING, Player::WHITE };
	board["f3"] = new Piece{ Piece::PAWN, Player::WHITE };

	board["a7"] = new Piece{ Piece::PAWN, Player::BLACK };
	board["a2"] = new Piece{ Piece::QUEEN,Player::BLACK };
	board["c2"] = new Piece{ Piece::ROOK, Player::BLACK };
	board["h8"] = new Piece{ Piece::ROOK, Player::BLACK };
	board["c8"] = new Piece{ Piece::KING, Player::BLACK };
	board["h7"] = new Piece{ Piece::PAWN, Player::BLACK };
	board["g6"] = new Piece{ Piece::PAWN, Player::BLACK };
	board["f5"] = new Piece{ Piece::PAWN, Player::BLACK };
}

//pinned bishop
void Board::customPos3() {
	clearBoard();
	for (char file = 'a'; file <= 'h'; ++file) {
		for (int rank = 1; rank <= 8; ++rank) {
			std::string position = std::string(1, file) + std::to_string(rank);
			board[position] = nullptr;
		}
	}

	board["g1"] = new Piece{ Piece::KING, Player::WHITE };
	board["f2"] = new Piece{ Piece::PAWN, Player::WHITE };
	board["g2"] = new Piece{ Piece::PAWN, Player::WHITE };
	board["h2"] = new Piece{ Piece::PAWN, Player::WHITE };
	board["a6"] = new Piece{ Piece::BISHOP, Player::WHITE };
	board["e4"] = new Piece{ Piece::QUEEN, Player::WHITE };

	board["a7"] = new Piece{ Piece::PAWN, Player::BLACK };
	board["b7"] = new Piece{ Piece::BISHOP, Player::BLACK };
	board["c7"] = new Piece{ Piece::PAWN, Player::BLACK };
	board["d7"] = new Piece{ Piece::PAWN, Player::BLACK };
	board["g7"] = new Piece{ Piece::PAWN, Player::BLACK };
	board["h7"] = new Piece{ Piece::PAWN, Player::BLACK };
	board["b6"] = new Piece{ Piece::PAWN, Player::BLACK };
	board["h8"] = new Piece{ Piece::ROOK, Player::BLACK };
	board["c8"] = new Piece{ Piece::KING, Player::BLACK };
}

//pinned knight
void Board::customPos4() {
	clearBoard();
	for (char file = 'a'; file <= 'h'; ++file) {
		for (int rank = 1; rank <= 8; ++rank) {
			std::string position = std::string(1, file) + std::to_string(rank);
			board[position] = nullptr;
		}
	}

	board["a2"] = new Piece{ Piece::PAWN, Player::WHITE };
	board["b2"] = new Piece{ Piece::PAWN, Player::WHITE };
	board["c2"] = new Piece{ Piece::PAWN, Player::WHITE };
	board["c3"] = new Piece{ Piece::KNIGHT, Player::WHITE };
	board["a1"] = new Piece{ Piece::ROOK, Player::WHITE };
	board["c1"] = new Piece{ Piece::QUEEN, Player::WHITE };
	board["e1"] = new Piece{ Piece::KING, Player::WHITE };
	board["h1"] = new Piece{ Piece::ROOK, Player::WHITE };
	board["d4"] = new Piece{ Piece::PAWN, Player::WHITE };
	board["f2"] = new Piece{ Piece::PAWN, Player::WHITE };
	board["g2"] = new Piece{ Piece::PAWN, Player::WHITE };
	board["h2"] = new Piece{ Piece::PAWN, Player::WHITE };

	board["a7"] = new Piece{ Piece::PAWN, Player::BLACK };
	board["b7"] = new Piece{ Piece::PAWN, Player::BLACK };
	board["f7"] = new Piece{ Piece::PAWN, Player::BLACK };
	board["g7"] = new Piece{ Piece::PAWN, Player::BLACK };
	board["h7"] = new Piece{ Piece::PAWN, Player::BLACK };
	board["f8"] = new Piece{ Piece::ROOK, Player::BLACK };
	board["g8"] = new Piece{ Piece::KING, Player::BLACK };
	board["a5"] = new Piece{ Piece::QUEEN, Player::BLACK };
	board["b4"] = new Piece{ Piece::BISHOP, Player::BLACK };
	board["e4"] = new Piece{ Piece::KNIGHT, Player::BLACK };
	board["d5"] = new Piece{ Piece::PAWN, Player::BLACK };

}

// enpassant and double move pawn
void Board::customPos5() {
	clearBoard();
	for (char file = 'a'; file <= 'h'; ++file) {
		for (int rank = 1; rank <= 8; ++rank) {
			std::string position = std::string(1, file) + std::to_string(rank);
			board[position] = nullptr;
		}
	}

	board["e1"] = new Piece{ Piece::KING, Player::WHITE };
	board["d5"] = new Piece{ Piece::PAWN, Player::WHITE };

	board["e8"] = new Piece{ Piece::KING, Player::BLACK };
	board["e7"] = new Piece{ Piece::PAWN, Player::BLACK };
}

// king moves
void Board::customPos6() {
	clearBoard();
	for (char file = 'a'; file <= 'h'; ++file) {
		for (int rank = 1; rank <= 8; ++rank) {
			std::string position = std::string(1, file) + std::to_string(rank);
			board[position] = nullptr;
		}
	}

	board["e4"] = new Piece{ Piece::KING, Player::WHITE };
	board["e5"] = new Piece{ Piece::BISHOP, Player::WHITE };
	board["c6"] = new Piece{ Piece::BISHOP, Player::WHITE };

	board["e6"] = new Piece{ Piece::KING, Player::BLACK };
}

// king moves in check
void Board::customPos7() {
	clearBoard();
	for (char file = 'a'; file <= 'h'; ++file) {
		for (int rank = 1; rank <= 8; ++rank) {
			std::string position = std::string(1, file) + std::to_string(rank);
			board[position] = nullptr;
		}
	}

	board["h1"] = new Piece{ Piece::KING, Player::WHITE };
	board["f4"] = new Piece{ Piece::QUEEN, Player::WHITE };
	board["f6"] = new Piece{ Piece::QUEEN, Player::WHITE };

	board["g5"] = new Piece{ Piece::KING, Player::BLACK };
}

// castle king being blocked
void Board::customPos8() {
	clearBoard();
	for (char file = 'a'; file <= 'h'; ++file) {
		for (int rank = 1; rank <= 8; ++rank) {
			std::string position = std::string(1, file) + std::to_string(rank);
			board[position] = nullptr;
		}
	}

	board["e4"] = new Piece{ Piece::KING, Player::WHITE };
	board["e5"] = new Piece{ Piece::BISHOP, Player::WHITE };
	board["c6"] = new Piece{ Piece::BISHOP, Player::WHITE };

	board["e6"] = new Piece{ Piece::KING, Player::BLACK };
}

// castle quuen blocked
void Board::customPos9() {
	clearBoard();
	for (char file = 'a'; file <= 'h'; ++file) {
		for (int rank = 1; rank <= 8; ++rank) {
			std::string position = std::string(1, file) + std::to_string(rank);
			board[position] = nullptr;
		}
	}

	board["e4"] = new Piece{ Piece::KING, Player::WHITE };
	board["e5"] = new Piece{ Piece::BISHOP, Player::WHITE };
	board["c6"] = new Piece{ Piece::BISHOP, Player::WHITE };

	board["e6"] = new Piece{ Piece::KING, Player::BLACK };
}

// kingmoves check can take
void Board::customPos10() {
	clearBoard();
	for (char file = 'a'; file <= 'h'; ++file) {
		for (int rank = 1; rank <= 8; ++rank) {
			std::string position = std::string(1, file) + std::to_string(rank);
			board[position] = nullptr;
		}
	}

	board["h1"] = new Piece{ Piece::KING, Player::WHITE };
	board["f4"] = new Piece{ Piece::QUEEN, Player::WHITE };
	board["f6"] = new Piece{ Piece::QUEEN, Player::WHITE };

	board["g5"] = new Piece{ Piece::KING, Player::BLACK };
	board["g6"] = new Piece{ Piece::KNIGHT, Player::BLACK };
	board["a7"] = new Piece{ Piece::KNIGHT, Player::BLACK };
}

// pawn double moves blocked
void Board::customPos11() {
	clearBoard();
	for (char file = 'a'; file <= 'h'; ++file) {
		for (int rank = 1; rank <= 8; ++rank) {
			std::string position = std::string(1, file) + std::to_string(rank);
			board[position] = nullptr;
		}
	}

	board["e8"] = new Piece{ Piece::KING, Player::BLACK };
	board["e7"] = new Piece{ Piece::PAWN, Player::BLACK };

	board["e3"] = new Piece{ Piece::KING, Player::WHITE };
	board["e4"] = new Piece{ Piece::PAWN, Player::WHITE };
	board["e5"] = new Piece{ Piece::PAWN, Player::WHITE };
}

// pawn takes 
void Board::customPos12() {
	clearBoard();
	for (char file = 'a'; file <= 'h'; ++file) {
		for (int rank = 1; rank <= 8; ++rank) {
			std::string position = std::string(1, file) + std::to_string(rank);
			board[position] = nullptr;
		}
	}

	board["e1"] = new Piece{ Piece::KING, Player::WHITE };
	board["e4"] = new Piece{ Piece::PAWN, Player::WHITE };
	board["d4"] = new Piece{ Piece::PAWN, Player::WHITE };

	board["e8"] = new Piece{ Piece::KING, Player::BLACK };
	board["e5"] = new Piece{ Piece::QUEEN, Player::BLACK };
	board["d5"] = new Piece{ Piece::PAWN, Player::BLACK };
}

