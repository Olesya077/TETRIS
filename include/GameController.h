#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "Field.h"
#include "ConsoleView.h"
#include "Figure.h"
#include "TerminalInput.h"
class GameController {
private:
    Field* field; 
    int prevFigureX;
    int prevFigureY; 
    Figure figure;
    ConsoleView view;
    TerminalInput input;
    bool gameRunning;
    int count;
    int FindDropDepth();
    void DropFigure();
    void CheckTerminalSize();
    bool gamePaused; 
    bool CanRotate();

public:
    GameController();
    ~GameController();
    void Input();
    void NewPosition();
    void run();
    bool CanMove(int dx, int dy);
    void UpdateField();
    void AutoMoveDown();
    bool GameMenu();
    void TerminalControl();
    void ShowPauseMenu();
};

#endif