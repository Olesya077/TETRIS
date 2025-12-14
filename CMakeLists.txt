cmake_minimum_required(VERSION 3.10)
project(TetrisGame LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)
add_executable(tetris_game
    src/main.cpp
    src/Figure.cpp
    src/Field.cpp
    src/ConsoleView.cpp
    src/GameController.cpp
    src/TerminalInput.cpp
    src/TerminalHelper.cpp
    src/Score.cpp
    src/Settings.cpp
    src/PictureField.cpp
)