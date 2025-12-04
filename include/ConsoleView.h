#ifndef CONSOLEVIEW_H
#define CONSOLEVIEW_H

#include "Field.h"
#include <vector>
#include <string>
using namespace std;
class ConsoleView {
public:
    ConsoleView();
    void ShowField(Field& field);
    void ShowFigure(Figure& oldFigure, Figure& figure, Field& field, int oldX, int oldY, int newX, int newY);
    string getRandomColor();
    void ShowPlacedFigure(Figure& figure, Field& field);
    void UpdateClearedLines(Field& field, int startRow);
    void ShowGhostFigure(Figure& figure, Field& field);
    void ClearGhostFigure(Figure& figure, Field& field);
};

#endif