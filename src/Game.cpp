#include "Game.h"
#include <iostream>
#include <fstream>

Game::Game() {
    record.clear();
}

Game::~Game() {

}

bool Game::getPawnPos(Move& m) {
    bool isCapture = (m.Notation.find('x') != std::string::npos);
    int direction = (m.color == Player::WHITE) ? 1 : -1;
    int doubleMoves = (m.color == Player::WHITE) ? 4 : 5;
    int epRank = (m.color == Player::WHITE) ? 5 : 4;
    int targetEpRank = (m.color == Player::WHITE) ? 6 : 3;
    int promotionRank = (m.color == Player::WHITE) ? 8 : 1;

    char fileFrom = m.target[0];
    int rankFrom = m.target[1] - '0' - direction;
    int targetRank = m.target[1] - '0';
    if (targetRank == promotionRank) {
        if (!(m.promotionType == Piece::Type::QUEEN || m.promotionType == Piece::Type::ROOK || m.promotionType == Piece::Type::KNIGHT || m.promotionType == Piece::Type::BISHOP)) {
            return false;
        }
    }

    m.parseFrom();


    if (!isCapture) {
        if (board.board[m.target] != nullptr) {
            return false;
        }
        m.from = std::string(1, fileFrom) + std::to_string(rankFrom);
        if (doubleMoves == targetRank) {
            if (board.board[m.from] != nullptr && board.board[m.from]->color == m.color) {
                m.pieceId = board.board[m.from]->id;
                return true;
            }
            rankFrom = rankFrom - direction;
            if (board.board[m.from] != nullptr) {
                // can't double move duee obstaclees;
                std::cout << " pawn can't double moves there obstacles" << std::endl;
                return false;
            }
            m.from = std::string(1, fileFrom) + std::to_string(rankFrom);
            if (board.board[m.from] == nullptr) {
                return false;
            }
            if (board.board[m.from]->color != m.color) {
                return false;
            }
            m.isPawnDoubleMoves = true;
            m.pieceId = board.board[m.from]->id;
            return true;
        }
        else {
            if (board.board[m.from] == nullptr) {
                return false;
            }
            if (board.board[m.from]->color != m.color) {
                return false;
            }
            m.pieceId = board.board[m.from]->id;
            return true;
        }
    }
    else {
        m.from = m.from + std::to_string(rankFrom);
        if (board.board[m.from] == nullptr) {
            return false;
        }
        if (board.board[m.target] == nullptr) {
            if (targetRank == targetEpRank) {
                std::string targetEp = std::string(1, m.target[0]) + std::to_string(targetRank-direction);
                if (board.board[targetEp] == nullptr) {
                    return false;
                }

                if (board.board[targetEp]->type != Piece::Type::PAWN) {
                    return false;
                }

                if (board.board[targetEp]->color == m.color) {
                    return false;
                }

                if (record.back().target != targetEp) {
                    return false;
                }

                if (record.back().pieceType != Piece::Type::PAWN) {
                    return false;
                }
                if (!record.back().isPawnDoubleMoves) {
                    return false;
                }
                if (!board.canAttackSquare(m.from, m.target)) {
                    return false;
                }

                m.isEnpassant = true;
                m.captureId = board.board[targetEp]->id;
                m.pieceId = board.board[m.from]->id;
                return true;
            }
        }
        else {
            if (board.board[m.target]->color == m.color) {
                return false;
            }
            if (!board.canAttackSquare(m.from, m.target)) {
                return false;
            }
            m.captureId = board.board[m.target]->id;
            m.pieceId = board.board[m.from]->id;
            return true;
        }
    }
    return false;
}

bool Game::getPos(Move& m) {
    // check if it is capturing

    if (m.pieceType == Piece::Type::PAWN) {
        if (getPawnPos(m)) {
            return true;
        }
        else {
            return false;
        }
    }

    if (board.board[m.target] != nullptr) {
        if (board.board[m.target]->color == m.color) {
            // can't capture his own piece
            return false;
        }

        m.captureId = board.board[m.target]->id;
        bool iscapt = m.Notation.find('x') != std::string::npos;

        if (!m.captureId || !iscapt) {
            return false;
        }
    }

    std::vector<std::string> possibleOrigins;
    Piece* tCapture = nullptr;

    if (m.captureId) {
        std::swap(tCapture, board.board[m.target]);
    }

    for (const auto& entry : board.board) {
        if (entry.second == nullptr) {
            continue;
        }
        if (entry.second->color == m.color && entry.second->type == m.pieceType) {
            if (board.canAttackSquare(entry.first, m.target)) {
                possibleOrigins.push_back(entry.first);
            }
        }
    }

    if (possibleOrigins.size() == 1) {
        m.from = possibleOrigins.front();
        m.pieceId = board.board[m.from]->id;
        if (m.captureId) {
            std::swap(board.board[m.target], tCapture);
        }
        return true;
    }

    if (possibleOrigins.size() > 1) {
        m.parseFrom();
        if (m.from.empty()) {
            if (m.captureId) {
                std::swap(board.board[m.target], tCapture);
            }
            return false;
        }


        if (m.from.length() == 1) {
            char squareChar = m.from[0];
            std::vector<std::string> possibleFrom;
            if (std::isdigit(squareChar)) {
                for (const auto& entry : board.board) {
                    char rankChar = entry.first[1];
                    if (entry.second == nullptr) {
                        continue;
                    }
                    if (entry.second->color == m.color && entry.second->type == m.pieceType && rankChar == squareChar) {
                        if (board.canAttackSquare(entry.first, m.target)) {
                            possibleFrom.push_back(entry.first);
                        }
                    }
                }
            }
            else if (std::isalpha(squareChar)) {
                for (const auto& entry : board.board) {
                    char fileChar = entry.first[0];
                    if (entry.second == nullptr) {
                        continue;
                    }
                    if (entry.second->color == m.color && entry.second->type == m.pieceType && fileChar == squareChar) {
                        if (board.canAttackSquare(entry.first, m.target)) {
                            possibleFrom.push_back(entry.first);
                        }
                    }
                }
            }
            if (m.captureId) {
                std::swap(board.board[m.target], tCapture);
            }

            if (possibleFrom.size() != 1) {
                return false;
            }

            m.from = possibleFrom.front();
            m.pieceId = board.board[m.from]->id;
            return true;
        }
        else if (m.from.length() == 2) {
            std::vector<std::string> possibleRank;
            std::vector<std::string> possibleFile;
            char fileChar = m.from[0];
            char rankChar = m.from[1];

            for (const auto& entry : board.board) {
                char tFile = entry.first[0];
                char tRank = entry.first[1];

                if (entry.second == nullptr) {
                    continue;
                }

                if (entry.second->color == m.color && entry.second->type == m.pieceType && fileChar == tFile) {
                    possibleFile.push_back(entry.first);
                }

                if (entry.second->color == m.color && entry.second->type == m.pieceType && rankChar == tRank) {
                    possibleRank.push_back(entry.first);
                }
            }

            if (m.captureId) {
                std::swap(board.board[m.target], tCapture);
            }

            if (possibleFile.size() < 2 && possibleRank.size() < 2) {
                return false;
            }

            m.pieceId = board.board[m.from]->id;
            return true;
        }
        if (m.captureId) {
            std::swap(board.board[m.target], tCapture);
        }
        return false;
    }
    if (m.captureId) {
        std::swap(board.board[m.target], tCapture);
    }
    return false;
}

bool Game::kingMove(Move& m) {
    Player& currentPlayer = (m.color == Player::Color::WHITE) ? white : black;
    Piece* tempCapture = nullptr;
    if (m.captureId) {
        std::swap(board.board[m.target], tempCapture);
    }
    if (!board.canAttackSquare(m.from, m.target)) {
        if (m.captureId) {
            std::swap(board.board[m.target], tempCapture);
        }
        std::cout << m.from << std::endl;
        std::cout << m.target << std::endl;
        return false;
    }
    std::swap(board.board[m.from], board.board[m.target]);

    if (currentPlayer.isCheck(&board)) {
        std::swap(board.board[m.from], board.board[m.target]);
        if (m.captureId) {
            std::swap(board.board[m.target], tempCapture);
        }
        return false;
    }

    if (m.captureId) {
        currentPlayer.capture.push_back(tempCapture);
    }
    return true;
}

bool Game::pawnMove(Move& m) {
    Player& currentPlayer = (m.color == Player::Color::WHITE) ? white : black;
    int direction = (m.color == Player::WHITE) ? 1 : -1;
    int targetRank = m.target[1] - '0';
    std::string targetEp = std::string(1, m.target[0]) + std::to_string(targetRank - direction);
    Piece* tempCapture = nullptr;
    if (m.captureId) {
        if (m.isEnpassant) {
            std::swap(board.board[targetEp], tempCapture);
        }
        else {
            std::swap(board.board[m.target], tempCapture);
        }
    }

    std::swap(board.board[m.from], board.board[m.target]);

    if (currentPlayer.isCheck(&board)) {
        std::swap(board.board[m.from], board.board[m.target]);
        if (m.isEnpassant) {
            std::swap(board.board[targetEp], tempCapture);
        }
        else {
            std::swap(board.board[m.target], tempCapture);
        }
        return false;
    }

    if (m.promotionType == 2) {
        board.board[m.target]->type = Piece::ROOK;
    }
    else if (m.promotionType == 3) {
        board.board[m.target]->type = Piece::KNIGHT;
    }
    else if (m.promotionType == 4) {
        board.board[m.target]->type = Piece::BISHOP;
    }
    else if (m.promotionType == 5) {
        board.board[m.target]->type = Piece::QUEEN;
    }
    if (m.captureId) {
        currentPlayer.capture.push_back(tempCapture);
    }
    return true;
}

bool Game::pieceMove(Move& m) {
    Player& currentPlayer = (m.color == Player::Color::WHITE) ? white : black;
    Piece* tempCapture = nullptr;

    if (m.captureId) {
        std::swap(board.board[m.target], tempCapture);
    }

    if (!board.canAttackSquare(m.from, m.target)) {

        std::swap(board.board[m.target], tempCapture);

        return false;
    }

    std::swap(board.board[m.from], board.board[m.target]);

    if (currentPlayer.isCheck(&board)) {
        std::swap(board.board[m.from], board.board[m.target]);
        if (m.captureId) {
            std::swap(board.board[m.target], tempCapture);
        }
        return false;
    }

    if (m.captureId) {
        currentPlayer.capture.push_back(tempCapture);
    }
    return true;
}

bool Game::makeMove(Move& m) {
    Player& currentPlayer = (m.color == Player::Color::WHITE) ? white : black;
    m.ecId = currentPlayer.isCheck(&board);
    if (m.isCastleKing) {
        if (m.ecId) {
            return false;
        }
        if (currentPlayer.kMove || currentPlayer.rhMove) {
            return false;
        }

        if (currentPlayer.color == Player::Color::WHITE) {
            if (board.board["e1"] == nullptr) {
                return false;
            }

            if (board.board["h1"] == nullptr) {
                return false;
            }

            if (board.board["e1"]->type != Piece::Type::KING || board.board["e1"]->color != currentPlayer.color) {
                return false;
            }

            if (board.board["h1"]->type != Piece::Type::ROOK || board.board["h1"]->color != currentPlayer.color) {
                return false;
            }

            if (board.board["f1"] != nullptr || board.board["g1"] != nullptr) {
                return false;
            }
            for (const auto& entry : board.board) {
                Piece* piece = entry.second;
                if (piece != nullptr && piece->color == Player::Color::WHITE) {
                    continue;
                }
                if (board.canAttackSquare(entry.first, "f1")) {
                    return false;
                }
                if (board.canAttackSquare(entry.first, "g1")) {
                    return false;
                }
            }

            std::swap(board.board["e1"], board.board["g1"]);
            std::swap(board.board["h1"], board.board["f1"]);
            return true;
        }
        else {
            if (board.board["e8"] == nullptr) {
                return false;
            }

            if (board.board["h8"] == nullptr) {
                return false;
            }

            if (board.board["e8"]->type != Piece::Type::KING || board.board["e8"]->color != currentPlayer.color) {
                return false;
            }

            if (board.board["h8"]->type != Piece::Type::ROOK || board.board["h8"]->color != currentPlayer.color) {
                return false;
            }

            if (board.board["f8"] != nullptr || board.board["g8"] != nullptr) {
                return false;
            }
            for (const auto& entry : board.board) {
                Piece* piece = entry.second;
                if (piece != nullptr && piece->color == Player::Color::BLACK) {
                    continue;
                }
                if (board.canAttackSquare(entry.first, "f8")) {
                    return false;
                }
                if (board.canAttackSquare(entry.first, "g8")) {
                    return false;
                }
            }

            std::swap(board.board["e8"], board.board["g8"]);
            std::swap(board.board["h8"], board.board["f8"]);
            return true;
        }
    }
    else if (m.isCastleQueen) {
        if (m.ecId) {
            return false;
        }
        if (currentPlayer.kMove || currentPlayer.raMove) {
            return false;
        }

        if (currentPlayer.color == Player::Color::WHITE) {
            if (board.board["e1"] == nullptr) {
                return false;
            }

            if (board.board["a1"] == nullptr) {
                return false;
            }

            if (board.board["e1"]->type != Piece::Type::KING || board.board["e1"]->color != currentPlayer.color) {
                return false;
            }

            if (board.board["a1"]->type != Piece::Type::ROOK || board.board["a1"]->color != currentPlayer.color) {
                return false;
            }

            if (board.board["b1"] != nullptr || board.board["c1"] != nullptr || board.board["d1"] != nullptr) {
                return false;
            }
            for (const auto& entry : board.board) {
                Piece* piece = entry.second;
                if (piece == nullptr) {
                    continue;
                }
                if (piece != nullptr && piece->color == Player::Color::WHITE) {
                    continue;
                }
                if (board.canAttackSquare(entry.first, "c1")) {
                    return false;
                }
                if (board.canAttackSquare(entry.first, "d1")) {
                    return false;
                }
            }

            std::swap(board.board["e1"], board.board["c1"]);
            std::swap(board.board["a1"], board.board["d1"]);
            return true;
        }
        else {
            if (board.board["e8"] == nullptr) {
                return false;
            }

            if (board.board["a8"] == nullptr) {
                return false;
            }

            if (board.board["e8"]->type != Piece::Type::KING || board.board["e8"]->color != currentPlayer.color) {
                return false;
            }

            if (board.board["a8"]->type != Piece::Type::ROOK || board.board["a8"]->color != currentPlayer.color) {
                return false;
            }

            if (board.board["b8"] != nullptr || board.board["c8"] != nullptr || board.board["d8"] != nullptr) {
                return false;
            }
            for (const auto& entry : board.board) {
                Piece* piece = entry.second;
                if (piece == nullptr) {
                    continue;
                }
                if (piece != nullptr && piece->color == Player::Color::BLACK) {
                    continue;
                }
                if (board.canAttackSquare(entry.first, "c8")) {
                    return false;
                }
                if (board.canAttackSquare(entry.first, "d8")) {
                    return false;
                }
            }

            std::swap(board.board["e8"], board.board["c8"]);
            std::swap(board.board["a8"], board.board["d8"]);
            return true;
        }
    }
    else {
        if (!getPos(m)) {
            return false;
        }
        if (m.pieceType == Piece::Type::KING) {
            if (kingMove(m)) {
                return true;
            }
            else {
                return false;
            }
        }
        else if (m.pieceType == Piece::Type::PAWN) {
            if (pawnMove(m)) {
                return true;
            }
            else {
                return false;
            }
        }
        else {
            if (pieceMove(m)) {
                return true;
            }
            else {
                return false;
            }
        }
    }
    return false;
}

void Game::gameMenu() {
    int choice;
    do {
        // Display menu options
        std::cout << "----- Game Menu -----" << std::endl;
        std::cout << "1. Play vs Bot" << std::endl;
        std::cout << "2. Play vs 2 Player" << std::endl;
        std::cout << "3. Quit" << std::endl;
        std::cout << "Enter your choice: ";

        // Get user choice
        std::cin >> choice;

        // Process user choice
        switch (choice) {
        case 1:
            vsBot();
            break;
        case 2:
            vs2Player();
            break;
        case 3:
            // Quit
            std::cout << "Exiting game..." << std::endl;
            break;
        default:
            std::cout << "Invalid choice! Please enter a valid option." << std::endl;
        }

    } while (choice != 3);
}

bool areOnSameDiagonal(const std::string& square1, const std::string& square2) {
    int fileDiff = std::abs(square1[0] - square2[0]);
    int rankDiff = std::abs(square1[1] - square2[1]);
    return fileDiff == rankDiff;
}

bool areOnSameRank(const std::string& square1, const std::string& square2) {
    return square1[1] == square2[1]; // Compare ranks (row numbers)
}

bool areOnSameFile(const std::string& square1, const std::string& square2) {
    return square1[0] == square2[0]; // Compare files (column letters)
}

std::vector<std::string> calculateGap(const std::string& square1, const std::string& square2) {
    std::vector<std::string> squaresBetween;
    if (areOnSameDiagonal(square1, square2)) {
        int file1 = square1[0] - 'a';
        int rank1 = square1[1] - '0';
        int file2 = square2[0] - 'a';
        int rank2 = square2[1] - '0';
        int fileDiff = file2 - file1;
        int rankDiff = rank2 - rank1;
        int fileDir = (fileDiff > 0) ? 1 : -1;
        int rankDir = (rankDiff > 0) ? 1 : -1;

        for (int file = file1 + fileDir, rank = rank1 + rankDir; file != file2; file += fileDir, rank += rankDir) {
            squaresBetween.push_back(std::string(1, 'a' + file) + std::to_string(rank));
        }
    }
    else if (areOnSameRank(square1, square2)) {
        int file1 = square1[0] - 'a';
        int file2 = square2[0] - 'a';
        int fileDiff = file2 - file1;
        int fileDir = (fileDiff > 0) ? 1 : -1;

        for (int file = file1 + fileDir; file != file2; file += fileDir) {
            squaresBetween.push_back(std::string(1, 'a' + file) + square1[1]);
        }
    }
    else if (areOnSameFile(square1, square2)) {
        // Calculate squares between two positions on the same file
        char file = square1[0];
        int rank = square1[1] - '0';
        int rank1 = square1[1] - '0';
        int rank2 = square2[1] - '0';
        int rankDiff = rank2 - rank1;
        int rankDir = (rankDiff > 0) ? 1 : -1;

        for (int rank = rank1 + rankDir; rank != rank2; rank += rankDir) {
            squaresBetween.push_back(std::string(1, file) + std::to_string(rank));
        }
    }
    return squaresBetween;
}

bool Game::isGameOver() {
    Player& currentPlayer = (record.size() % 2 == 0) ? white : black;
    int ecId = currentPlayer.isCheck(&board);
    std::vector <std::string> kSquareMove;
    std::vector<std::string> pSquareBlock;
    std::vector<std::string> pCanAttack;
    std::vector<std::pair<int, int>> directions = {
    {1, 0}, {1, 1}, {0, 1}, {-1, 1},
    {-1, 0}, {-1, -1}, {0, -1}, {1, -1}
    };
    std::string kPos = "";
    Piece* enemyPiece = nullptr;
    std::string ePos = "";

    if (!ecId) {
        return false;
    }
    record.back().isCheck = true;

    for (const auto& entry : board.board) {
        if (entry.second == nullptr) {
            continue;
        }

        if (entry.second->color == currentPlayer.color && entry.second->type == Piece::Type::KING) {
            kPos = entry.first;
        }

        if (entry.second->id == ecId) {
            ePos = entry.first;
        }
    }

    if (kPos.empty() || ePos.empty()) {
        // return exit game...
        return false;
    }

    for (const auto& dir : directions) {
        // Calculate the position of the adjacent square
        char file = kPos[0] + dir.first;
        int rank = kPos[1] - '0' + dir.second;

        // Check if the square is within the bounds of the board
        if (file >= 'a' && file <= 'h' && rank >= 1 && rank <= 8) {
            std::string square = std::string(1, file) + std::to_string(rank);

            if (board.board[square] == nullptr) {
                std::swap(board.board[square], board.board[kPos]);
                if (!currentPlayer.isCheck(&board)) {
                    kSquareMove.push_back(square);
                }
                std::swap(board.board[square], board.board[kPos]);
            }
            else {
                if (board.board[square]->color != currentPlayer.color) {
                    Piece* tempP = nullptr;
                    std::swap(board.board[square], tempP);
                    std::swap(board.board[square], board.board[kPos]);
                    if (!currentPlayer.isCheck(&board)) {
                        kSquareMove.push_back(square);
                    }
                    std::swap(board.board[square], board.board[kPos]);
                    std::swap(board.board[square], tempP);
                }
            }
        }
    }

    if (!kSquareMove.empty()) {
        std::cout << kSquareMove.size() << "King square move square " << std::endl;
        return false;
    }

    std::swap(board.board[ePos], enemyPiece);

    // Check if any piece can capture the attacking piece
    for (const auto& entry : board.board) {
        if (entry.second == nullptr) {
            continue;
        }
        if (entry.second->color == currentPlayer.color && entry.second->type != Piece::Type::KING) {
            if (board.canAttackSquare(entry.first, ePos)) {
                pCanAttack.push_back(entry.first);
            }
        }
    }

    if (!pCanAttack.empty()) {
        std::swap(board.board[ePos], enemyPiece);
        return false; 
    }

    // Check if any piece can block the attack
    bool sameDiagonal = areOnSameDiagonal(kPos, ePos);
    bool sameRank = areOnSameRank(kPos, ePos);
    bool sameFile = areOnSameFile(kPos, ePos);

    std::vector<std::string> gap = calculateGap(kPos, ePos);
    for (const auto& entry : board.board) {
        if (entry.second == nullptr) {
            continue;
        }
        if (entry.second->color == currentPlayer.color) {
            for (int a = 0; a < gap.size(); a++) {
                if (board.canAttackSquare(entry.first, gap[a])) {
                    pSquareBlock.push_back(entry.first);
                }
            }
        }
    }

    if (!pSquareBlock.empty()) {
        std::cout << pSquareBlock.size() << "piece block size square " << std::endl;
        std::swap(board.board[ePos], enemyPiece);
        return false; 
    }
    record.back().isMate = true;
    // If none of the above conditions are met, it's checkmate and the game is over
    return true;
}

void Game::displayNotation() {
    for (size_t i = 0; i < record.size(); ++i) {
        size_t number = i + 1;
        std::cout <<number <<". " << record[i].getNotation() << std::endl;
    }
}

void Game::saveNotation() {
    std::string filename;
    std::cout << "Enter filename to save notation (e.g., game.pgn): ";
    std::cin >> filename;
    filename += ".pgn";

    std::ofstream pgnFile(filename);
    if (pgnFile.is_open()) {
        // Write PGN header (optional)
        // pgnFile << "[Event \"Chess Game\"]\n";
        // pgnFile << "[Site \"Location\"]\n";
        // pgnFile << "[Date \"YYYY.MM.DD\"]\n";
        // pgnFile << "[White \"Player1\"]\n";
        // pgnFile << "[Black \"Player2\"]\n";
        // pgnFile << "[Result \"*\"]\n\n"; // Placeholder for game result

        // Write move notation
        for (auto& m : record) {
            pgnFile << m.getNotation()<< " ";
            // Optionally, add newline or other separators
        }

        // Close the file
        pgnFile.close();
        std::cout << "Game notation saved to '" << filename << "'.\n";
    }
    else {
        std::cerr << "Error: Unable to open file '" << filename << "' for writing.\n";
    }
}

void Game::displayResult() {
    system("cls");
    std::string winner = (record.size() % 2 == 1) ? "White" : "Black";
    std::cout << winner << " wins by CheckMate on: " << record.back().getNotation() << std::endl;
    int choice;
    do {
        std::cout << "Menu:" << std::endl;
        std::cout << "1. Check notation" << std::endl;
        std::cout << "2. Save notation" << std::endl;
        std::cout << "3. Back to menu" << std::endl;
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
        case 1:
            displayNotation();
            break;
        case 2:
            saveNotation();
            break;
        case 3:
            system("cls");
            return; // Exit the menu
        default:
            std::cout << "Invalid choice! Please try again." << std::endl;
        }
    } while (choice != 3);

}

void Game::vsBot() {
    std::cout << "It's Coming Soon...." << std::endl;
    return;
}

void Game::vs2Player() {
    board.setBoard(white,black);
    while (!isGameOver()) {
        board.printBoard();
        // first need to check whose turn it is
        Player& currentPlayer = (record.size() % 2 == 0) ? white : black;

        std::cout << (currentPlayer.color == Player::WHITE ? "White" : "Black") << "'s to move: ";

        Move move;
        move.color = currentPlayer.color;
        move.number = record.size() + 1;

        while (true) {
            std::string m = "";
            std::cin >> m;
            if (move.inputMove(m)) {
                if (makeMove(move)) {
                    record.push_back(move);
                    break;
                }
            }
            std::cout << (currentPlayer.color == Player::WHITE ? "White" : "Black") << "'s move is Illegal Please enter a valid move: ";
        }

        if (isGameOver()) {
            displayResult();
            return;
        }
    }
}

void Game::start() {
    gameMenu();
}