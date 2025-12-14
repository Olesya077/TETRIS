/**
 * @file Figure.cpp
 * @brief Реализация фигур Тетриса
 * 
 * Содержит матрицы форм и логику вращения для всех 7 типов фигур.
 * Каждая фигура имеет предопределенные состояния вращения.
 */
#include "Figure.h"
#include <algorithm>
#include <iostream>

Figure::Figure() : matrix(4, false), width(2), height(2), rotationState(0) {}

bool Figure::getchar(int i, int j) {
    if (i >= 0 && i < height && j >= 0 && j < width) {
        return matrix[i * width + j];
    }
    return false;
}

int Figure::getstartx() { return startx; }
int Figure::getstarty() { return starty; }

void Figure::setPosition(int x, int y) {
    startx = x;
    starty = y;
}

int Figure::getHeight() { return height; }
int Figure::getWidth() { return width; }
string Figure::getcolor() { return color; }

void Figure::rotate() {
    /**
     * @brief Вращает фигуру на 90 градусов по часовой стрелке
     * @note Каждый тип фигуры имеет свои предопределенные матрицы вращения
     *       Фигура O не вращается
     *       Фигура I имеет особую логику смещения
     */
    rotationState = (rotationState + 1) % 4;

    if (color == "\x1b[35m") {
        rotationState = 0;
        return;
    }

    if (color == "\x1b[33m") {
        std::vector<std::vector<bool>> tStates = {
            {false, true, false, true, true, true, false, false, false},
            {false, true, false, false, true, true, false, true, false},
            {false, false, false, true, true, true, false, true, false},
            {false, true, false, true, true, false, false, true, false}
        };
        matrix = tStates[rotationState];
        width = 3;
        height = 3;
    }
    else if (color == "\x1b[36m") { 
        std::vector<std::vector<bool>> lStates = {
            {true, false, false, true, false, false, true, true, false},
            {false, false, false, true, true, true, true, false, false},
            {false, true, true, false, false, true, false, false, true},
            {false, false, false, false, false, true, true, true, true}
        };
        matrix = lStates[rotationState];
        width = 3;
        height = 3;
    }
    else if (color == "\x1b[37m") {
        std::vector<std::vector<bool>> jStates = {
            {false, false, true, false, false, true, false, true, true},
            {false, false, false, true, true, true, false, false, true},
            {true, true, false, true, false, false, true, false, false},
            {true, false, false, true, true, true, false, false, false}
        };
        matrix = jStates[rotationState];
        width = 3;
        height = 3;
    }
    else if (color == "\x1b[32m") {
        std::vector<std::vector<bool>> sStates = {
            {false, true, true, true, true, false, false, false, false},
            {false, true, false, false, true, true, false, false, true},
            {false, false, false, false, true, true, true, true, false},
            {true, false, false, true, true, false, false, true, false}
        };
        matrix = sStates[rotationState];
        width = 3;
        height = 3;
    }
    else if (color == "\x1b[31m") {
        std::vector<std::vector<bool>> zStates = {
            {true, true, false, false, true, true, false, false, false},
            {false, false, true, false, true, true, false, true, false},
            {false, false, false, true, true, false, false, true, true},
            {false, true, false, true, true, false, true, false, false}
        };
        matrix = zStates[rotationState];
        width = 3;
        height = 3;
    }
    else if (color == "\x1b[34m") {
        std::vector<std::vector<bool>> iStates = {
            {false, false, false, false, true, true, true, true, false, false, false, false, false, false, false, false},
            {false, false, true, false, false, false, true, false, false, false, true, false, false, false, true, false},
            {false, false, false, false, false, false, false, false, true, true, true, true, false, false, false, false},
            {false, true, false, false, false, true, false, false, false, true, false, false, false, true, false, false}
        };
        matrix = iStates[rotationState];
        width = 4;
        height = 4;
    }
}
FigureO::FigureO() {
    matrix = std::vector<bool>(4, true);
    width = 2;
    height = 2;
    color = "\x1b[35m";
    rotationState = 0;
}

FigureL::FigureL() {
    matrix = {true, false, false,
              true, false, false,
              true, true, false};
    width = 3;
    height = 3;
    color = "\x1b[36m";
    rotationState = 0;
}

FigureT::FigureT() {
    matrix = {false, true, false,
              true, true, true,
              false, false, false};
    width = 3;
    height = 3;
    color = "\x1b[33m";
    rotationState = 0;
}

FigureI::FigureI() {
    matrix = {false, false, false, false,
              true, true, true, true,
              false, false, false, false,
              false, false, false, false};
    width = 4;
    height = 4;
    color = "\x1b[34m";
    rotationState = 0;
}

FigureS::FigureS() {
    matrix = {false, true, true,
              true, true, false,
              false, false, false};
    width = 3;
    height = 3;
    color = "\x1b[32m";
    rotationState = 0;
}

FigureZ::FigureZ() {
    matrix = {true, true, false,
              false, true, true,
              false, false, false};
    width = 3;
    height = 3;
    color = "\x1b[31m";
    rotationState = 0;
}

FigureJ::FigureJ() {
    matrix = {false, false, true,
              false, false, true,
              false, true, true};
    width = 3;
    height = 3;
    color = "\x1b[37m";
    rotationState = 0;
}
