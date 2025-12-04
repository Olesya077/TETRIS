#include "GameController.h"
#include "TerminalInput.h"  
#include "TerminalHelper.h"  
#include <iostream>   
#include <unistd.h>

GameController::GameController() : field(nullptr), gameRunning(true), count(1), gamePaused(false) {
    figure.setPosition(10, 1);
    TerminalHelper::initResizeHandler();
}

GameController::~GameController() {
    delete field;
    field = nullptr;
}

void GameController::AutoMoveDown() {
    if (CanMove(0, 1)) {
        figure.setPosition(figure.getstartx(), figure.getstarty() + 1);
    }
}


void GameController::DropFigure() {
    if (!field) return;
    int oldX = figure.getstartx();
    int oldY = figure.getstarty();
    Figure oldFigure = figure;

    int dropDepth = 0;
    while (CanMove(0, dropDepth + 1)) {
        dropDepth++;
    }

    if (dropDepth > 0) {
        figure.setPosition(figure.getstartx(), figure.getstarty() + dropDepth);
        int newX = figure.getstartx();
        int newY = figure.getstarty();
        view.ShowFigure(oldFigure, figure, *field, oldX, oldY, newX, newY);
        usleep(50000);
    }
    NewPosition();
}

void GameController::ShowPauseMenu() {
    TerminalHelper::saveScreen();
    TerminalHelper::clearScreen();
     std::cout << "\033[s";
    std::cout << "=== ПАУЗА ===" << std::endl;
    std::cout << "Игра приостановлена" << std::endl;
    std::cout << "Нажмите r чтобы вернуться в игру" << std::endl;
    std::cout << "Нажмите q чтобы выйти" << std::endl;
    std::cout << "Нажмите e чтобы выйти в главное меню" << std::endl;
}

bool GameController::CanRotate() {
    if (!field) return false;
    int oldX = figure.getstartx();
    int oldY = figure.getstarty();
    std::string oldColor = figure.getcolor();
    Figure testFigure;
    if (oldColor == "\x1b[35m") testFigure = FigureO();
    else if (oldColor == "\x1b[36m") testFigure = FigureL();
    else if (oldColor == "\x1b[33m") testFigure = FigureT();
    else if (oldColor == "\x1b[34m") testFigure = FigureI();
    else if (oldColor == "\x1b[32m") testFigure = FigureS();
    else if (oldColor == "\x1b[31m") testFigure = FigureZ();
    else if (oldColor == "\x1b[37m") testFigure = FigureJ();
    testFigure.setPosition(oldX, oldY);
    testFigure.rotate();
    if (oldColor == "\x1b[34m") {
        if (figure.getRotationState() % 2 == 0) {
            testFigure.setPosition(oldX - 1, oldY);
        } else {
            testFigure.setPosition(oldX + 1, oldY);
        }
    }
    bool canRotate = true;
    int x = testFigure.getstartx();
    int y = testFigure.getstarty();
    
    for (int i = 0; i < testFigure.getHeight() && canRotate; i++) {
        for (int j = 0; j < testFigure.getWidth() && canRotate; j++) {
            if (testFigure.getchar(i, j)) {
                int fieldX = x + j;
                int fieldY = y + i;

                if (!field->isValidPosition(fieldY, fieldX)) {
                    canRotate = false;
                }
                
                if (field->getch(fieldY, fieldX)) {
                    canRotate = false;
                }
            }
        }
    }
    
    return canRotate;
}

void GameController::Input() {
    char c = input.getInput();
    
    if (gamePaused) {
        switch(c) {
            case 'r':
                gamePaused = false;
                TerminalHelper::restoreScreen();
                view.ShowField(*field);
                view.ShowPlacedFigure(figure, *field);
                break;
            case 'q':
                TerminalHelper::restoreScreen();
                gameRunning = false;
                break;
            case 'e':
                gamePaused = false;
                TerminalHelper::restoreScreen();
                delete field;
                field = nullptr;
                TerminalHelper::clearScreen();
                if (GameMenu()) {
                    view.ShowField(*field);
                    figure = FigureO();
                } else {
                    gameRunning = false;
                }
                break;
        }
        return;
    }
    
    if (c == ' ') {
        if (CanRotate()) {
            figure.rotate();
        }
        return;
    }
    if (!CanMove(0, 1)) {
        switch(c) {
            case 'a':
            case 'd':
            case 's':
                break;
            case ' ':
                if (CanRotate()) {
                    figure.rotate();
                }
                break;
            case 'q':
                gameRunning = false;
                break;
            case 'e':
                gamePaused = true;
                ShowPauseMenu();
                break;
        }
    } else {
        switch(c) {
            case 'a':
                if (CanMove(-1, 0)) {
                    figure.setPosition(figure.getstartx() - 1, figure.getstarty());
                }
                break;
            case 'd':
                if (CanMove(1, 0)) {
                    figure.setPosition(figure.getstartx() + 1, figure.getstarty());
                }
                break;
            case 's':
                if (CanMove(0, 1)) {
                    figure.setPosition(figure.getstartx(), figure.getstarty() + 1);
                }
                break;
            case 'w':
                DropFigure();
                break;
            case ' ':
                if (CanRotate()) {
                    figure.rotate();
                }
                break;
            case 'q':
                gameRunning = false;
                break;
            case 'e':
                gamePaused = true;
                ShowPauseMenu();
                break;
        }
    }
}

bool GameController::CanMove(int dx, int dy) {
    if (!field) return false;
    
    int newX = figure.getstartx() + dx;
    int newY = figure.getstarty() + dy;
 
    for (int i = 0; i < figure.getHeight(); i++) {
        for (int j = 0; j < figure.getWidth(); j++) {
            if (figure.getchar(i, j)) {
                int fieldX = newX + j;
                int fieldY = newY + i;

                if (!field->isValidPosition(fieldY, fieldX)) {
                    return false;
                }
                
                if (field->getch(fieldY, fieldX)) {
                    return false;
                }
            }
        }
    }
    return true;
}

void GameController::NewPosition() {
    if (!field) return;
        
    if (CanMove(0, 1)) {
        //figure.setPosition(figure.getstartx(), figure.getstarty() + 1);
    } else {
        field->placeFigure(figure);
        view.ShowPlacedFigure(figure, *field);
        int figureType = count % 7;
        switch(figureType) {
            case 0:
                figure = FigureO();
                break;
            case 1:
                figure = FigureL();
                break;
            case 2:
                figure = FigureT();
                break;
            case 3:
                figure = FigureI();
                break;
            case 4:
                figure = FigureS();
                break;
            case 5:
                figure = FigureZ();
                break;
            case 6:
                figure = FigureJ();
                break;
        }
        count++;
        
        int linesCleared = field->clearFullLines();
        if (linesCleared > 0) {
            std::cout << "Количество очищенных линий: " << linesCleared << std::endl;
            view.ShowField(*field);
        }
        
        figure.setPosition(10, 1);
        
        if (!CanMove(0, 0)) {
            gameRunning = false;
            std::cout << "GAME OVER!" << std::endl;
        }
    }
}

bool GameController::GameMenu(){
    std::cout << "            Добро пожаловать в Тетрис! " << std::endl;
    std::cout << "Нажмите 1, чтобы продолжить"<< std::endl;

    char c;
    do {
        c = input.getInput();
        usleep(10000);
    } while (c != '1');
    
    TerminalHelper::clearScreen();
    std::cout << "Нажмите s - чтобы выбрать стандартное поле, h - расширяющееся поле (ведро)" << std::endl;
    std::cout << "Управление в игре:" << std::endl;
    std::cout << "a - влево, d - вправо, s - вниз, w - мгновенное падение" << std::endl;
    std::cout << "ПРОБЕЛ - поворот фигуры" << std::endl;
    std::cout << "e - пауза/меню, q - выход" << std::endl;

    while (true) {
        c = input.getInput();
        if (c == 's') {
            field = new Field(); 
            std::cout << "Выбрано стандартное поле!" << std::endl;
            usleep(1000000);
            return true;
        } else if (c == 'h') {
            field = new BucketField();
            std::cout << "Выбрано расширяющееся поле-ведро!" << std::endl;
            usleep(1000000);
            return true;
        }
        usleep(10000);
    }
    return false;
}

void GameController::run() {
    if (!TerminalHelper::isTerminalSizeValid(24, 24)) {
        int rows, cols;
        TerminalHelper::getCurrentSize(rows, cols);
        std::cerr << "Размер терминала слишком маленький! Минимум: 24x24" << std::endl;
        return;
    }
    
    TerminalHelper::clearScreen();
    if(GameMenu()) {
        view.ShowField(*field);
        figure = FigureO();
        while (gameRunning) {
            if (TerminalHelper::wasResized()) {
                if (!TerminalHelper::isTerminalSizeValid(24, 24)) {
                    TerminalHelper::clearScreen();
                    int rows, cols;
                    TerminalHelper::getCurrentSize(rows, cols);
                    std::cout << "Размер терминала слишком маленький!" << std::endl;
                    std::cout << "Текущий размер: " << cols << "x" << rows << std::endl;
                    std::cout << "Требуется: 24x24" << std::endl;
                    usleep(2000000);
                } else if (!gamePaused) {
                    view.ShowField(*field);
                }
            }
            
            if (!TerminalHelper::isTerminalSizeValid(24, 24)) {
                TerminalHelper::clearScreen();
                int rows, cols;
                TerminalHelper::getCurrentSize(rows, cols);
                usleep(100000);
                continue;
            }
            if (gamePaused) {
                Input();
                usleep(100000);
                continue;
            }
            
            int oldX = figure.getstartx();
            int oldY = figure.getstarty();
            Figure oldFigure = figure;
            Input();
            if (gamePaused) {
                continue;
            }
            
            NewPosition();
            usleep(200000);
            int newX = figure.getstartx();
            int newY = figure.getstarty(); 

            view.ShowFigure(oldFigure, figure, *field, oldX, oldY, newX, newY);
            usleep(200000);
        }
    }
    TerminalHelper::restoreScreen();
    TerminalHelper::moveCursorToSafePosition();
}