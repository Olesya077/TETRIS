#include "Field.h"
#include <iostream>
#include <algorithm>

Field::Field() : fieldmatrix(22 * 22, false), fieldcolors(22 * 22, ""), fieldWidth(22), fieldHeight(22) {
    for (int i = 0; i < 22; i++) {
        setch(21, i, true, " ");
        setch(i, 0, true, " ");
        setch(i, 21, true, " ");
    }
}

bool Field::isValidPosition(int row, int col) {
    return (row >= 0 && row < fieldHeight && col >= 0 && col < fieldWidth);
}

std::string Field::getColor(int i, int j) {
    if (i >= 0 && i < fieldHeight && j >= 0 && j < fieldWidth) {
        return fieldcolors[i * fieldWidth + j];
    }
    return "\x1b[37m"; 
}

BucketField::BucketField() {
    fieldHeight = 22;
    fieldWidth = 22;
    initializeBucketShape();
}

void BucketField::initializeBucketShape() {
    rowWidths.resize(fieldHeight);
    
    int minWidth = 6;
    int maxWidth = 22;
    
    for (int row = 0; row < fieldHeight; row++) {
        rowWidths[row] = minWidth + (maxWidth - minWidth) * row / (fieldHeight - 1);
    }
    
    fieldmatrix.clear();
    fieldmatrix.resize(fieldHeight * fieldWidth, false);
    
    for (int row = 0; row < fieldHeight; row++) {
        int width = rowWidths[row];
        int leftBound = (fieldWidth - width) / 2;
        int rightBound = leftBound + width;
        
        for (int col = 0; col < fieldWidth; col++) {
            bool isWall = (col < leftBound) || (col >= rightBound);
            setch(row, col, isWall);
        }
    }

    for (int col = 0; col < fieldWidth; col++) {
        setch(fieldHeight - 1, col, true);
    }
}

int BucketField::getRowWidth(int row) {
    if (row >= 0 && row < fieldHeight) {
        return rowWidths[row];
    }
    return fieldWidth;
}

bool BucketField::isValidPosition(int row, int col) {
    if (row < 0 || row >= fieldHeight || col < 0 || col >= fieldWidth) {
        return false;
    }
    
    int width = getRowWidth(row);
    int leftBound = (fieldWidth - width) / 2;
    int rightBound = leftBound + width;
    
    return col >= leftBound && col < rightBound;
}

void BucketField::placeFigure(Figure& figure) {
    int x = figure.getstartx();
    int y = figure.getstarty();

    for (int i = 0; i < figure.getHeight(); i++) {
        for (int j = 0; j < figure.getWidth(); j++) {
            if (figure.getchar(i, j)) {
                int fieldx = x + j;
                int fieldy = y + i;
                
                if (isValidPosition(fieldy, fieldx)) {
                    setch(fieldy, fieldx, true, figure.getcolor());
                }
            }
        }
    }
}


int BucketField::clearFullLines() {
    int linesCleared = 0;

    for (int row = fieldHeight - 2; row >= 1; row--) { 
        bool lineFull = true;

        int width = getRowWidth(row);
        int leftBound = (fieldWidth - width) / 2;
        int rightBound = leftBound + width;
        
        for (int col = leftBound; col < rightBound; col++) {
            if (!getch(row, col)) {
                lineFull = false;
                break;
            }
        }

        if (lineFull) {
            linesCleared++;
            
            for (int moveRow = row; moveRow >= 1; moveRow--) {
                int moveWidth = getRowWidth(moveRow);
                int moveLeftBound = (fieldWidth - moveWidth) / 2;
                int moveRightBound = moveLeftBound + moveWidth;
                
                int prevWidth = getRowWidth(moveRow - 1);
                int prevLeftBound = (fieldWidth - prevWidth) / 2;
                int prevRightBound = prevLeftBound + prevWidth;
                
                for (int col = moveLeftBound; col < moveRightBound; col++) {
                    bool value = false;
                    std::string color = "";
                    
                    if (moveRow > 0 && col >= prevLeftBound && col < prevRightBound) {
                        value = getch(moveRow - 1, col);
                        color = getColor(moveRow - 1, col);
                    }
                    setch(moveRow, col, value, color);
                }
            }
            
            int topWidth = getRowWidth(0);
            int topLeftBound = (fieldWidth - topWidth) / 2;
            int topRightBound = topLeftBound + topWidth;
            for (int col = topLeftBound; col < topRightBound; col++) {
                setch(0, col, false);
            }
            
            row++;
        }
    }
    
    return linesCleared;
}
void Field::placeFigure(Figure& figure) {
    int x = figure.getstartx();
    int y = figure.getstarty();

    for (int i = 0; i < figure.getHeight(); i++) {
        for (int j = 0; j < figure.getWidth(); j++) {
            int fieldx = x + j;
            int fieldy = y + i;
            if (figure.getchar(i, j)) {
                setch(fieldy, fieldx, true, figure.getcolor());
            }
        }
    }
}


bool Field::getch(int i, int j) {
    if (i >= 0 && i < fieldHeight && j >= 0 && j < fieldWidth) {
        return fieldmatrix[i * fieldWidth + j];
    }
    return true;
}

void Field::setch(int i, int j, bool value, const std::string& color) {
    if (i >= 0 && i < fieldHeight && j >= 0 && j < fieldWidth) {
        fieldmatrix[i * fieldWidth + j] = value;
        if (!color.empty()) {
            fieldcolors[i * fieldWidth + j] = color;
        } else if (!value) {
            fieldcolors[i * fieldWidth + j] = "";
        }
    }
}
int Field::getHeight() { return fieldHeight; }
int Field::getWidth() { return fieldWidth; }

int Field::clearFullLines() {
    int linesCleared = 0;

    for (int row = fieldHeight - 2; row >= 1; row--) { 
        bool lineFull = true;

        for (int col = 1; col < fieldWidth - 1; col++) {
            if (!getch(row, col)) {
                lineFull = false;
                break;
            }
        }

        if (lineFull) {
            linesCleared++;
            for (int moveRow = row; moveRow >= 1; moveRow--) {
                for (int col = 1; col < fieldWidth - 1; col++) {
                    bool value = getch(moveRow - 1, col);
                    std::string color = getColor(moveRow - 1, col);
                    setch(moveRow, col, value, color);
                }
            }
            for (int col = 1; col < fieldWidth - 1; col++) {
                setch(0, col, false);
            }
            row++;
        }
    }
    
    return linesCleared;
}
