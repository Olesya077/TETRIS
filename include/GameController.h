#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "Field.h"
#include "ConsoleView.h"
#include "Figure.h"
#include "TerminalInput.h"
#include "Score.h"
#include "Settings.h"

class GameController {
private:
    Field* field;
    int prevFigureX;
    int prevFigureY;
    Figure figure;
    ConsoleView view;
    TerminalInput input;
    GameScore scoreSystem;
    Settings* settings;
    bool gameRunning;
    int count;
    int score;
    int linesClearedTotal;
    std::string playerName;
    bool gamePaused;

public:
    GameController();
    ~GameController();
    void Input();
    void NewPosition();
    void run();
    bool CanMove(int dx, int dy);
    void UpdateField();
    void AutoMoveDown();
    void TerminalControl();
    void ShowPauseMenu();
    void ShowSettingsMenu(bool fromPause = false);
    void ShowMainSettingsMenu();
    void ShowControlSettings();
    void ShowCurrentSettings();
    void addPoints(int points);
    bool GameMenu();
    void updateLevel();
    void showLevelInfo();

private:
    void DropFigure();
    bool CanRotate();
    void showScore();
    char mapInput(char c);
};

#endif