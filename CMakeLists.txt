cmake_minimum_required(VERSION 3.10)
project(PlayChessNotation)

# Add executable target
add_executable(PlayChessNotation
    src/main.cpp
    src/Board.cpp
    src/Bot.cpp
    src/Game.cpp
    src/Move.cpp
    src/Piece.cpp
    src/Player.cpp
)

# Add include directories
target_include_directories(PlayChessNotation PUBLIC
    include
)
