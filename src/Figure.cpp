#include "Figure.h"
#include <algorithm>
Figure::Figure() : matrix(4, false), width(2), height(2) {}

bool Figure::getchar(int i, int j) {
    return matrix[i * width + j];
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
    if (width == 2 && height == 2) {
        std::vector<bool> temp = matrix;

        matrix[0] = temp[2];
        matrix[1] = temp[0];  
        matrix[2] = temp[3];
        matrix[3] = temp[1];
        rotationState = (rotationState + 1) % 4;
        return;
    }
    rotationState = (rotationState + 1) % 4;
}

FigureO::FigureO() {
    matrix = std::vector<bool>(4, true);
    width = 2;
    height = 2;
    color = "\x1b[35m";
}

FigureL::FigureL() {
    matrix = std::vector<bool>(4, false);
    matrix[0] = true;
    matrix[1] = false;
    matrix[2] = true;
    matrix[3] = true;  
    width = 2;
    height = 2;
    color = "\x1b[36m";
}