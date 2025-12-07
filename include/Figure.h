#ifndef FIGURE_H
#define FIGURE_H

#include <vector>
#include <string>
using namespace std;

class Figure {
protected:
    std::vector<bool> matrix;
    string color;
    int width = 2;             
    int height = 2;
    int startx = 10;
    int starty = 1;
    int rotationState = 0;

public:
    Figure();
    const std::vector<bool>& getMatrix() const { return matrix; }
    void setMatrix(const std::vector<bool>& newMatrix, int newWidth, int newHeight) {
        matrix = newMatrix;
        width = newWidth;
        height = newHeight;}
    bool getchar(int i, int j);
    int getstartx();
    int getstarty();
    int getHeight();
    int getWidth();
    string getcolor();
    int getRotationState() const { return rotationState; };

    void setPosition(int x, int y);
    void rotate();
};

class FigureO : public Figure {
public:
    FigureO();
};

class FigureL : public Figure {
public:
    FigureL();
};

class FigureT : public Figure {
public:
    FigureT();
};

class FigureI : public Figure {
public:
    FigureI();
};

class FigureS : public Figure {
public:
    FigureS();
};

class FigureZ : public Figure {
public:
    FigureZ();
};

class FigureJ : public Figure {
public:
    FigureJ();
};
#endif