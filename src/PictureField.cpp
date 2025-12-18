/**
 * @file PictureField.cpp
 * @brief Реализация поля для режима "Собери картинку"
 * 
 * Содержит логику создания различных картинок и проверки их сборки.
 */
#include "PictureField.h"
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include "TerminalHelper.h"

PictureField::PictureField() : PictureField(PICTURE_SQUARE) {}

PictureField::PictureField(int type) 
    : targetPicture(22 * 22, false),
      currentPicture(22 * 22, false),
      pictureColors(22 * 22, ""),
      currentPictureType(type),
      gameOver(false) {
    
    for (int i = 0; i < 22; i++) {
        setch(21, i, true, " ");
        setch(i, 0, true, " ");
        setch(i, 21, true, " ");
    }
    loadPicture(type);
}

void PictureField::loadPicture(int type) {
    currentPictureType = type;
    
    for (int i = 0; i < 22 * 22; i++) {
        targetPicture[i] = false;
        currentPicture[i] = false;
    }
    
    switch(type) {
        case PICTURE_SQUARE:
            drawSquare();
            break;
        case PICTURE_DIAMOND:
            drawDiamond();
            break;
        case PICTURE_HOUSE:
            drawHouse();
            break;
        case PICTURE_ARROW:
            drawArrow();
            break;
        case PICTURE_BUTTERFLY:
            drawButterfly();
            break;
        case PICTURE_STAR:
            drawStar();
            break;
        case PICTURE_TRIANGLE:
            drawTriangle();
            break;    
        default:
            drawSquare();
    }
}

void PictureField::drawSquare() {
    for (int i = 0; i < 22 * 22; i++) {
        targetPicture[i] = false;
    }
    
    int squareWidth = 10;
    int squareHeight = 10;
    int startRow = 21 - squareHeight;
    int startCol = 6;
    
    for (int row = startRow; row < startRow + squareHeight; row++) {
        for (int col = startCol; col < startCol + squareWidth; col++) {
            if (isValidPosition(row, col)) {
                targetPicture[row * 22 + col] = true;
            }
        }
    }
}

void PictureField::drawDiamond() {
    
}

void PictureField::drawHouse() {
    
}

void PictureField::drawArrow() {
    
}

void PictureField::drawButterfly() {
    
}

void PictureField::drawStar() {

}

void PictureField::drawTriangle() {
    for (int i = 0; i < 22 * 22; i++) {
        targetPicture[i] = false;
    }
    
    int triangleHeight = 16;
    int triangleBase = 19;
    
    int floorRow = 20;
    int startRow = floorRow - triangleHeight + 1;
    int startCol = (22 - triangleBase) / 2;
    
    for (int row = 0; row < triangleHeight; row++) {
        int rowWidth = (row * triangleBase) / triangleHeight;
        
        if (rowWidth < 1) rowWidth = 1;
        
        int colStart = startCol + (triangleBase - rowWidth) / 2;
        int colEnd = colStart + rowWidth;
        
        for (int col = colStart; col < colEnd; col++) {
            int fieldRow = startRow + row;
            int fieldCol = col;
            
            if (fieldRow >= 0 && fieldRow < 22 && fieldCol >= 0 && fieldCol < 22) {
                targetPicture[fieldRow * 22 + fieldCol] = true;
            }
        }
    }
}

std::string PictureField::getPictureName() const {
    switch(currentPictureType) {
        case PICTURE_SQUARE: return "Квадрат";
        case PICTURE_DIAMOND: return "Ромб";
        case PICTURE_HOUSE: return "Домик";
        case PICTURE_ARROW: return "Стрелка";
        case PICTURE_BUTTERFLY: return "Бабочка";
        case PICTURE_STAR: return "Звезда";
        case PICTURE_TRIANGLE: return "Треугольник";
        default: return "Неизвестно";
    }
}



void PictureField::placeFigure(Figure& figure) {
    int x = figure.getstartx();
    int y = figure.getstarty();
    bool placedInTarget = false;
    bool touchedBorder = false;
    
    for (int i = 0; i < figure.getHeight(); i++) {
        for (int j = 0; j < figure.getWidth(); j++) {
            if (figure.getchar(i, j)) {
                int fieldx = x + j;
                int fieldy = y + i;
                
                if (isValidPosition(fieldy, fieldx)) {
                    if (isInTargetArea(fieldy, fieldx)) {
                        placedInTarget = true;
                        currentPicture[fieldy * 22 + fieldx] = true;
                        pictureColors[fieldy * 22 + fieldx] = figure.getcolor();
                        setch(fieldy, fieldx, true, figure.getcolor());
                    } else {
                        touchedBorder = true;
                        setch(fieldy, fieldx, true, figure.getcolor());
                    }
                } else {
                    touchedBorder = true;
                }
            }
        }
    }
    
    if (placedInTarget) {
        if (isPictureComplete()) {
            TerminalHelper::moveCursorTo(27, 0);
            TerminalHelper::clearCurrentLine();
            std::cout << "\x1b[32m" << "ПОЗДРАВЛЯЕМ! Вы собрали картинку: " 
                      << getPictureName() << "!" << "\x1b[0m";
            std::cout.flush();
            usleep(3000000);
            gameOver = true;
        }
    }
    
    if (touchedBorder) {
        TerminalHelper::moveCursorTo(27, 0);
        TerminalHelper::clearCurrentLine();
        std::cout << "\x1b[31m" << "ИГРА ОКОНЧЕНА! Часть фигуры вышла за границы картинки." << "\x1b[0m";
        std::cout.flush();
        usleep(3000000);
        gameOver = true;
    }
}

void PictureField::resetGame() {
    gameOver = false;
    for (int i = 0; i < 22 * 22; i++) {
        currentPicture[i] = false;
        pictureColors[i] = "";
    }
}

bool PictureField::isValidPosition(int row, int col) {
    return (row >= 0 && row < 22 && col >= 0 && col < 22);
}

bool PictureField::isInTargetArea(int row, int col) const {
    if (row >= 0 && row < 22 && col >= 0 && col < 22) {
        return targetPicture[row * 22 + col];
    }
    return false;
}

bool PictureField::isPictureComplete() const {
    for (int i = 0; i < 22 * 22; i++) {
        if (targetPicture[i] && !currentPicture[i]) {
            return false;
        }
    }
    return true;
}