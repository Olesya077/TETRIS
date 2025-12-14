#include "ConsoleView.h"
#include <iostream>
#include "TerminalHelper.h"
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <set>

using namespace std;

ConsoleView::ConsoleView() {}

void ConsoleView::ShowField(Field& field) {
    if (!TerminalHelper::isTerminalSizeValid(24, 48)) {
        return;
    }
    
    TerminalHelper::clearScreen();
    TerminalHelper::moveCursorTo(0, 0);
    for (int i = 0; i < field.getHeight(); i++) {
        for (int j = 0; j < field.getWidth(); j++) {
            if (field.getch(i, j)) {
                std::string color = field.getColor(i, j);
                bool isBoundary = (i == 21) || (j == 0) || (j == 21) || 
                                 (color == " ") || color.empty();
                if (isBoundary) {
                    std::cout << "\x1b[37m" << "██" << "\x1b[0m";
                } else {
                    std::cout << color << "██" << "\x1b[0m";
                }
            }
            else {
                std::cout << "  ";
            }
        }
        std::cout << std::endl;
    }
    std::cout.flush();
}


void ConsoleView::ShowPlacedFigure(Figure& figure, Field& field) {
    if (!TerminalHelper::isTerminalSizeValid(24, 48)) {
        return;
    }

    int x = figure.getstartx();
    int y = figure.getstarty();

    for (int i = 0; i < figure.getHeight(); i++) {
        for (int j = 0; j < figure.getWidth(); j++) {
            if (figure.getchar(i, j)) {
                int fieldX = x + j;
                int fieldY = y + i;
                
                if (fieldY >= 0 && fieldY < field.getHeight() && 
                    fieldX >= 0 && fieldX < field.getWidth()) {
                    
                    TerminalHelper::moveCursorTo(fieldY, fieldX * 2);
                    std::cout << figure.getcolor() << "██" << "\x1b[0m";
                }
            }
        }
    }

    TerminalHelper::moveCursorToSafePosition();
    std::cout.flush();
}

void ConsoleView::ShowGhostFigure(Figure& figure, Field& field) {
    if (!TerminalHelper::isTerminalSizeValid(24, 48)) {
        return;
    }
    
    PictureField* pictureField = dynamic_cast<PictureField*>(&field);
    
    int dropDepth = 0;
    int currentX = figure.getstartx();
    int currentY = figure.getstarty();
    while (true) {
        bool canMoveDown = true;
        int x = figure.getstartx();
        int y = figure.getstarty();
        
        for (int i = 0; i < figure.getHeight() && canMoveDown; i++) {
            for (int j = 0; j < figure.getWidth() && canMoveDown; j++) {
                if (figure.getchar(i, j)) {
                    int fieldX = x + j;
                    int fieldY = y + i + dropDepth + 1;
                    
                    if (!field.isValidPosition(fieldY, fieldX) || field.getch(fieldY, fieldX)) {
                        canMoveDown = false;
                    }
                }
            }
        }
        
        if (canMoveDown) {
            dropDepth++;
        } else {
            break;
        }
    }
    
    if (dropDepth > 0) {
        int ghostX = currentX;
        int ghostY = currentY + dropDepth;
        
        for (int i = 0; i < figure.getHeight(); i++) {
            for (int j = 0; j < figure.getWidth(); j++) {
                if (figure.getchar(i, j)) {
                    int fieldX = ghostX + j;
                    int fieldY = ghostY + i;
                    bool isUnderCurrentFigure = false;
                    for (int fi = 0; fi < figure.getHeight() && !isUnderCurrentFigure; fi++) {
                        for (int fj = 0; fj < figure.getWidth() && !isUnderCurrentFigure; fj++) {
                            if (figure.getchar(fi, fj)) {
                                int currentFieldX = currentX + fj;
                                int currentFieldY = currentY + fi;
                                if (currentFieldX == fieldX && currentFieldY == fieldY) {
                                    isUnderCurrentFigure = true;
                                }
                            }
                        }
                    }
                    
                    if (fieldY >= 0 && fieldY < field.getHeight() && 
                        fieldX >= 0 && fieldX < field.getWidth() &&
                        !field.getch(fieldY, fieldX) && 
                        !isUnderCurrentFigure) {
                        
                        TerminalHelper::moveCursorTo(fieldY, fieldX * 2);
                        std::cout << "\x1b[90m" << "▓▓" << "\x1b[0m";
                    }
                }
            }
        }
    }
}

void ConsoleView::ClearGhostFigure(Figure& figure, Field& field) {
    if (!TerminalHelper::isTerminalSizeValid(24, 48)) {
        return;
    }
    
    PictureField* pictureField = dynamic_cast<PictureField*>(&field);
    
    int dropDepth = 0;
    while (true) {
        bool canMoveDown = true;
        int x = figure.getstartx();
        int y = figure.getstarty();
        
        for (int i = 0; i < figure.getHeight() && canMoveDown; i++) {
            for (int j = 0; j < figure.getWidth() && canMoveDown; j++) {
                if (figure.getchar(i, j)) {
                    int fieldX = x + j;
                    int fieldY = y + i + dropDepth + 1;
                    
                    if (!field.isValidPosition(fieldY, fieldX) || field.getch(fieldY, fieldX)) {
                        canMoveDown = false;
                    }
                }
            }
        }
        
        if (canMoveDown) {
            dropDepth++;
        } else {
            break;
        }
    }
    
    if (dropDepth > 0) {
        int ghostX = figure.getstartx();
        int ghostY = figure.getstarty() + dropDepth;

        for (int i = 0; i < figure.getHeight(); i++) {
            for (int j = 0; j < figure.getWidth(); j++) {
                if (figure.getchar(i, j)) {
                    int fieldX = ghostX + j;
                    int fieldY = ghostY + i;
                    
                    if (fieldY >= 0 && fieldY < field.getHeight() && 
                        fieldX >= 0 && fieldX < field.getWidth() &&
                        !field.getch(fieldY, fieldX)) {
                        
                        TerminalHelper::moveCursorTo(fieldY, fieldX * 2);
                        if (pictureField && pictureField->isInTargetArea(fieldY, fieldX)) {
                            std::cout << "\x1b[90m" << "▒▒" << "\x1b[0m";
                        } else {
                            std::cout << "  ";
                        }
                    }
                }
            }
        }
    }
}
void ConsoleView::ShowPictureField(Field& field) {
    if (!TerminalHelper::isTerminalSizeValid(24, 48)) {
        return;
    }
    
    TerminalHelper::clearScreen();
    TerminalHelper::moveCursorTo(0, 0);
    
    PictureField* pictureField = dynamic_cast<PictureField*>(&field);
    
    for (int i = 0; i < field.getHeight(); i++) {
        for (int j = 0; j < field.getWidth(); j++) {
            bool isWall = (i == 21) || (j == 0) || (j == 21);
            
            if (isWall) {
                std::cout << "\x1b[37m" << "██" << "\x1b[0m";
            }
            else if (field.getch(i, j)) {
                std::string color = field.getColor(i, j);
                if (!color.empty() && color != " ") {
                    std::cout << color << "██" << "\x1b[0m";
                } else {
                    std::cout << "\x1b[37m" << "██" << "\x1b[0m";
                }
            }
            else if (pictureField && pictureField->isInTargetArea(i, j)) {
                if (!field.getch(i, j)) {
                    std::cout << "\x1b[90m" << "▒▒" << "\x1b[0m";
                }
            }
            else {
                std::cout << "  ";
            }
        }
        std::cout << std::endl;
    }
    std::cout.flush();
}
void ConsoleView::ShowFigure(Figure& oldFigure, Figure& figure, Field& field, int oldX, int oldY, int newX, int newY) {
    if (!TerminalHelper::isTerminalSizeValid(24, 48)) {
        return;
    }
    
    PictureField* pictureField = dynamic_cast<PictureField*>(&field);
    for (int i = 0; i < oldFigure.getHeight(); i++) {
        for (int j = 0; j < oldFigure.getWidth(); j++) {
            if (oldFigure.getchar(i, j)) {
                int fieldX = oldX + j;
                int fieldY = oldY + i;
                
                if (fieldY >= 0 && fieldY < field.getHeight() && 
                    fieldX >= 0 && fieldX < field.getWidth() &&
                    !field.getch(fieldY, fieldX)) {
                    TerminalHelper::moveCursorTo(fieldY, fieldX * 2);
                    if (pictureField && pictureField->isInTargetArea(fieldY, fieldX)) {
                        std::cout << "\x1b[90m" << "▒▒" << "\x1b[0m";
                    } else {
                        std::cout << "  ";
                    }
                }
            }
        }
    }
    for (int i = 0; i < figure.getHeight(); i++) {
        for (int j = 0; j < figure.getWidth(); j++) {
            if (figure.getchar(i, j)) {
                int fieldX = newX + j;
                int fieldY = newY + i;
                
                if (fieldY >= 0 && fieldY < field.getHeight() && 
                    fieldX >= 0 && fieldX < field.getWidth() &&
                    !field.getch(fieldY, fieldX)) { 
                    
                    TerminalHelper::moveCursorTo(fieldY, fieldX * 2);
                    std::cout << figure.getcolor() << "██" << "\x1b[0m";
                }
            }
        }
    }
    ShowGhostFigure(figure, field);
    TerminalHelper::moveCursorToSafePosition();
    std::cout.flush();
}