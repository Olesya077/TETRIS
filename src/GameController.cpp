/**
 * @file GameController.cpp
 * @brief Реализация главного контроллера игры Тетрис
 * 
 * Содержит реализацию игровой логики, обработки ввода, меню и состояний игры.
 */
#include "GameController.h"
#include "TerminalInput.h"
#include "TerminalHelper.h"
#include "PictureField.h"

#include <iostream>
#include <unistd.h>
#include <algorithm>
#include <cctype>
#include <termios.h> 
#include <time.h>

GameController::GameController() : 
    field(nullptr), 
    prevFigureX(0),
    prevFigureY(0),
    figure(),
    view(),
    input(),
    scoreSystem(),
    settings(Settings::getInstance()),
    gameRunning(true),
    count(1),
    score(0),
    linesClearedTotal(0),
    playerName("Player"),
    gamePaused(false),
    isPictureMode(false),
    nameEntered(false)
{
    /**
     * @brief Инициализация контроллера
     * @note Настраивает начальную позицию фигуры и терминал
     */
    figure.setPosition(10, 1);
    TerminalHelper::initResizeHandler();
    TerminalHelper::saveScreen();
    TerminalHelper::clearScreen();
}

void GameController::GetPlayerName(){
    /**
     * @brief Запрашивает имя игрока
     * @note Включает отображение курсора и эхо ввода
     * @warning Временно меняет настройки терминала
     */
    if (nameEntered && !playerName.empty() && playerName != "Player") {
        return;
    }
    TerminalHelper::restoreScreen();
    TerminalHelper::showCursor();
        struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag |= ECHO | ICANON;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    
    std::cout << "Введите ваше имя: ";
    std::cout.flush();
    
    std::string inputLine;
    std::getline(std::cin, inputLine);
    
    if (!inputLine.empty()) {
        playerName = inputLine;
    } else {
        playerName = "Player";
    }
    
    nameEntered = true;
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    TerminalHelper::hideCursor();
    TerminalHelper::enableAlternateBuffer();
    TerminalHelper::clearScreen();
}

void GameController::returnToMenu() {
    if (field) {
        delete field;
        field = nullptr;
    }
    
    score = 0;
    linesClearedTotal = 0;
    settings->setLevel(1);
    isPictureMode = false;
    
    TerminalHelper::clearScreen();
    gamePaused = false;
    
    if (GameMenu()) {
        gameRunning = true;
        
        if (isPictureMode) {
            view.ShowPictureField(*field);
        } else {
            view.ShowField(*field);
        }
        
        figure = FigureO();
        if (isPictureMode) {
            PictureField* pictureField = dynamic_cast<PictureField*>(field);
            if (pictureField) {
                pictureField->resetGame();
                int startX = (field->getWidth() - figure.getWidth()) / 2;
                figure.setPosition(startX, 1);
            }
        } else {
            figure.setPosition(10, 1);
        }
        if (isPictureMode) {
        view.ShowPictureField(*field);
        }
        
        showScore();
        showLevelInfo();
        
        if (isPictureMode) {
            PictureField* pictureField = dynamic_cast<PictureField*>(field);
            if (pictureField) {
            }
        }
    } else {
        gameRunning = false;
    }
}

GameController::~GameController() {
    delete field;
    field = nullptr;
    Settings::destroyInstance();
    TerminalHelper::restoreScreen();
}

void GameController::AutoMoveDown() {
}

void GameController::DropFigure() {
    /**
     * @brief Выполняет быстрое падение фигуры
     * @note Рассчитывает глубину падения и начисляет бонусные очки
     */
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
        usleep(10000);
        
        int dropPoints = Settings::getDropPointsForLevel(settings->getLevel()) * dropDepth;
        addPoints(dropPoints);
        int messageY = field->getHeight() + 6;
        TerminalHelper::moveCursorTo(messageY, 0);
        TerminalHelper::clearCurrentLine();
        std::cout << "Дроп: +" << dropPoints << " очков";
    }
    NewPosition();
}

void GameController::Input() {
    char c = input.getInputWithArrows();
    if (c == 0) return;
    if (gamePaused) {
        switch(c) {
            case 'r':
                gamePaused = false;
                if (isPictureMode) {
                    view.ShowPictureField(*field);
                } else {
                    view.ShowField(*field);
                }
                view.ShowPlacedFigure(figure, *field);
                showScore();
                showLevelInfo();
                if (isPictureMode) {
                    PictureField* pictureField = (PictureField*)field;
                }
                break;
            case 'q':
                gameRunning = false;
                return;
            case 'n':
                gamePaused = false;
                delete field;
                field = nullptr;
                TerminalHelper::clearScreen();
            if (GameMenu()) {
            if (isPictureMode) {
             view.ShowPictureField(*field);
                } else {
            view.ShowField(*field);
                 }
        figure = FigureO();
        showScore();
        showLevelInfo();
        if (isPictureMode) {
            PictureField* pictureField = (PictureField*)field;
            if (pictureField) {
                view.ShowPictureField(*field);
            }
                }
            } else {
            gameRunning = false;
            }
            break;
            case 's':
                ShowSettingsMenu(true);
                break;
            case 'v':
                TerminalHelper::clearScreen();
                scoreSystem.displayScores();
                std::cout << "\nНажмите любую клавишу для возврата...";
                input.getInput();
                ShowPauseMenu();
                break;
        }
        return;
    }
    
    if (c == settings->getControl("LEFT") || c == '<') {
        if (CanMove(-1, 1)) {
            view.ClearGhostFigure(figure, *field);
            figure.setPosition(figure.getstartx() - 1, figure.getstarty()+1);
        }
    }
    else if (c == settings->getControl("RIGHT") || c == '>') {
        if (CanMove(1, 1)) {
            view.ClearGhostFigure(figure, *field);
            figure.setPosition(figure.getstartx() + 1, figure.getstarty()+1);
        }
    }
    else if (c == settings->getControl("DOWN") || c == 'v') {
        if (CanMove(0, 1)) {
            figure.setPosition(figure.getstartx(), figure.getstarty() + 1);
            addPoints(40 * settings->getLevel());
            showScore();
        }
    }
    else if (c == settings->getControl("DROP") || c == '^') {
        DropFigure();
    }
    else if (c == settings->getControl("ROTATE")) {
        if (CanRotate()) {
            view.ClearGhostFigure(figure, *field);
            figure.rotate();
            if (CanMove(0, 1)) {
            figure.setPosition(figure.getstartx(), figure.getstarty() + 1);
        }
        }
    }
    else if (c == settings->getControl("PAUSE")) {
        gamePaused = true;
        ShowPauseMenu();
    }
    else if (c == settings->getControl("QUIT")) {
        gameRunning = false;
    }
}

void GameController::ShowPauseMenu() {
    TerminalHelper::moveCursorTo(0, 0);
    TerminalHelper::clearScreen();
    std::cout << "=== ПАУЗА ===" << std::endl;
    std::cout << "Игрок: " << playerName << " | Текущий счет: " << score << std::endl;
    std::cout << "Уровень: " << settings->getLevel() << " | Линий очищено: " << linesClearedTotal << std::endl;
    std::cout << "----------------------------" << std::endl;
    std::cout << "Нажмите r чтобы вернуться в игру" << std::endl;
    std::cout << "Нажмите n чтобы начать новую игру" << std::endl;
    std::cout << "Нажмите s чтобы открыть настройки" << std::endl;
    std::cout << "Нажмите v чтобы посмотреть рекорды" << std::endl;
    std::cout << "Нажмите q чтобы выйти" << std::endl;
    std::cout.flush();
}

void GameController::ShowSettingsMenu(bool fromPause) {
    TerminalHelper::clearScreen();
    std::cout << "=== НАСТРОЙКИ ===" << std::endl;
    
    if (fromPause) {
        std::cout << "\n1. Изменить управление" << std::endl;
        std::cout << "2. Просмотреть текущие настройки" << std::endl;
        std::cout << "3. Вернуться в меню паузы" << std::endl;
    } else {
        std::cout << "\n1. Изменить управление" << std::endl;
        std::cout << "2. Просмотреть текущие настройки" << std::endl;
        std::cout << "3. Сбросить настройки по умолчанию" << std::endl;
        std::cout << "4. Вернуться в главное меню" << std::endl;
    }
    
    char choice;
    while (true) {
    choice = input.getInput();
    
    if (choice == '1') {
        ShowControlSettings();
        ShowSettingsMenu(fromPause);
        return;
    }
    else if (choice == '2') {
        ShowCurrentSettings();
        std::cout << "\nНажмите любую клавишу для продолжения...";
        input.getInput();
        ShowSettingsMenu(fromPause);
        return;
    }
    else if (choice == '3') {
        if (fromPause) {
            ShowPauseMenu();
            return;
        } else {
            settings->resetToDefaults();
            std::cout << "\nНастройки сброшены к значениям по умолчанию!" << std::endl;
            usleep(1000000);
            ShowMainSettingsMenu();
            return;
        }
    }
    else if (choice == '4' && !fromPause) {
        return;
    }
    else if (fromPause && (choice == '0' || choice == 'r')) {
        return;
    }
    else if (!fromPause && (choice == '0' || choice == 'q')) {
        return;
    }
    
    usleep(10000);
}
}

void GameController::ShowControlSettings() {
    TerminalHelper::clearScreen();
std::cout << "=== ИЗМЕНЕНИЕ УПРАВЛЕНИЯ ===" << std::endl;
    std::cout << "----------------------------" << std::endl;
    
    auto actions = settings->getAvailableActions();
    
    std::cout << "Доступные действия:" << std::endl;
    for (size_t i = 0; i < actions.size(); i++) {
        char key = settings->getControl(actions[i]);
        std::string keyStr;
        if (key == '^' || key == 'v' || key == '<' || key == '>') {
            //eyStr = Settings::arrowToString(key);
        } else if (key == ' ') {
            keyStr = "ПРОБЕЛ";
        } else {
            keyStr = std::string(1, key);
        }
        std::cout << (i+1) << ". " << actions[i] << " (" << keyStr << ")" << std::endl;
    }
    
    /*std::cout << "\nЗанятые клавиши: ";
    for (char key : occupiedKeys) {
        if (key != ' ') {
            std::cout << key << " ";
        }
    }*/
    std::cout << "\n*Системные клавиши (нельзя изменить): 1, 2, 3, 4, s, h, r, n, v, q" << std::endl;
    std::cout << "Нажмите 0, чтобы выйти" << std::endl;
    
    std::cout << "\nВведите номер действия для изменения (или 0 для возврата): ";

char actionChoice;
while (true) {
    actionChoice = input.getInput();
    if (actionChoice == '0') {
        return;
    }
    if (actionChoice == 'q' || actionChoice == 'r') {
        return;
    }
    
    int actionIndex = actionChoice - '1';
    if (actionIndex >= 0 && actionIndex < (int)actions.size()) {
        std::string selectedAction = actions[actionIndex];
        
        std::cout << "\nВыбрано: " << selectedAction << std::endl;
        std::cout << "Текущая клавиша: ";
        char currentKey = settings->getControl(selectedAction);
        if (currentKey == ' ') {
            std::cout << "ПРОБЕЛ" << std::endl;
        } else {
            std::cout << currentKey << std::endl;
        }
        
        std::cout << "Введите новую клавишу (или 0 для отмены): ";
        
        char newKey;
        while (true) {
            newKey = input.getInput();
            if (newKey != 0) {
                break;
            }
            usleep(10000);
        }
        
        if (newKey == '0') {
            std::cout << "\nОтменено" << std::endl;
            usleep(1000000);
            ShowControlSettings();
            return;
        }
        
        newKey = std::tolower(newKey);
        
        if (settings->setControl(selectedAction, newKey)) {
            std::cout << "\nНастройка сохранена! Новая клавиша: ";
            if (newKey == ' ') {
                std::cout << "ПРОБЕЛ";
            } else {
                std::cout << newKey;
            }
            std::cout << std::endl;
        } else {
            std::cout << "\nОшибка! Эта клавиша уже занята или недопустима." << std::endl;
        }
        
        usleep(1500000);
        ShowControlSettings();
        return;
    }
    
    usleep(10000);
}
}

void GameController::ShowCurrentSettings() {
    TerminalHelper::clearScreen();
    std::cout << "=== ТЕКУЩИЕ НАСТРОЙКИ ===" << std::endl;
    std::cout << "----------------------------" << std::endl;
    
    auto actions = settings->getAvailableActions();
    
    std::cout << "Управление:" << std::endl;
    for (const auto& action : actions) {
        char key = settings->getControl(action);
        std::string keyStr = (key == ' ') ? "ПРОБЕЛ" : std::string(1, key);
        std::cout << "  " << action << ": " << keyStr << std::endl;
    }
    
    std::cout << "\nСистемные настройки:" << std::endl;
    std::cout << "  Текущий уровень: " << settings->getLevel() << std::endl;
    std::cout << "  Скорость падения: " << Settings::getSpeedForLevel(settings->getLevel()) 
              << " мкс" << std::endl;
    std::cout << "  Баллы за дроп: " << Settings::getDropPointsForLevel(settings->getLevel()) 
              << " за клетку" << std::endl;
}

void GameController::ShowMainSettingsMenu() {
    TerminalHelper::clearScreen();
    std::cout << "=== НАСТРОЙКИ ТЕТРИС ===" << std::endl;
    std::cout << "==========================" << std::endl;
    std::cout << "\n1. Управление" << std::endl;
    std::cout << "2. Просмотреть текущие настройки" << std::endl;
    std::cout << "3. Сбросить все настройки" << std::endl;
    std::cout << "4. Вернуться в главное меню" << std::endl;
    
    char choice;
while (true) {
    choice = input.getInput();
    
    if (choice == '1') {
        ShowControlSettings();
        ShowMainSettingsMenu();
        return;
    }
    else if (choice == '2') {
        ShowCurrentSettings();
        std::cout << "\n\nНажмите любую клавишу для продолжения...";
        input.getInput();
        ShowMainSettingsMenu();
        return;
    }
    else if (choice == '3') {
        TerminalHelper::clearScreen();
        std::cout << "Вы уверены, что хотите сбросить все настройки?" << std::endl;
        std::cout << "1. Да, сбросить настройки" << std::endl;
        std::cout << "2. Нет, вернуться назад" << std::endl;
        
        char confirm;
        while (true) {
            confirm = input.getInput();
            if (confirm == '1') {
                settings->resetToDefaults();
                std::cout << "\nВсе настройки сброшены!" << std::endl;
                usleep(1000000);
                ShowMainSettingsMenu();
                return;
            } else if (confirm == '2') {
                ShowMainSettingsMenu();
                return;
            } else if (confirm == '0' || confirm == 'q') {
                ShowMainSettingsMenu();
                return;
            }
            usleep(10000);
        }
    }
    else if (choice == '4') {
    return;
    }
    else if (choice == '0' || choice == 'q') {
        return;
    }
    
    usleep(10000);
}
}

bool GameController::CanRotate() {
    /**
     * @brief Проверяет возможность поворота текущей фигуры
     * @return true если поворот возможен без столкновений
     * @note Для фигуры I учитывает смещение при вращении
     */
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
    for (int i = 0; i < figure.getRotationState(); i++) {
        testFigure.rotate();
    }
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
    /**
     * @brief Обновляет позицию фигуры или создает новую
     * @note Вызывается когда фигура не может двигаться вниз
     *       Размещает фигуру на поле, проверяет линии, создает новую фигуру
     */
    if (!field) return;
        
    if (CanMove(0, 1)) {
        return;
    } 
    
    field->placeFigure(figure);
    view.ShowPlacedFigure(figure, *field);
    
    if (isPictureMode) {
        PictureField* pictureField = dynamic_cast<PictureField*>(field);
        if (pictureField) {
            if (pictureField->isGameOver()) {
                showGameOverScreen(true);
                return;
            }
        }
    } else {
        int linesCleared = field->clearFullLines();
if (linesCleared > 0) {
    linesClearedTotal += linesCleared;
    
    int points = 0;
    switch(linesCleared) {
        case 1: points = 100; break;
        case 2: points = 300; break;
        case 3: points = 500; break;
        case 4: points = 800; break;
    }
    points *= settings->getLevel();
    
    addPoints(points);
    
    int messageY = field->getHeight() + 6; 
    TerminalHelper::moveCursorTo(messageY, 0);
    TerminalHelper::clearCurrentLine();
    std::cout << "Очищено: " << linesCleared << " линий | +" << points << " очков";
    
    showScore();
    updateLevel();
    view.ShowField(*field);
    showScore();
    showLevelInfo();
    TerminalHelper::moveCursorTo(messageY, 0);
    TerminalHelper::clearCurrentLine();
    std::cout << "Очищено: " << linesCleared << " линий | +" << points << " очков";
    view.ShowGhostFigure(figure, *field);
    
    TerminalHelper::moveCursorToSafePosition();
    std::cout.flush();
}
    }
        int figureType;
        figureType = rand() % 7;

    switch(figureType) {
        case 0: figure = FigureO(); break;
        case 1: figure = FigureL(); break;
        case 2: figure = FigureT(); break;
        case 3: figure = FigureI(); break;
        case 4: figure = FigureS(); break;
        case 5: figure = FigureZ(); break;
        case 6: figure = FigureJ(); break;
    }
    if (isPictureMode) {
        int startX = (field->getWidth() - figure.getWidth()) / 2;
        figure.setPosition(startX, 1);
    } else {
        figure.setPosition(10, 1);
    }
    
    if (!CanMove(0, 0)) {
        showGameOverScreen(false);
    }
}

void GameController::showGameOverScreen(bool isPictureModeGameOver) {
    TerminalHelper::clearScreen();
    
    if (isPictureModeGameOver) {
        PictureField* pictureField = dynamic_cast<PictureField*>(field);
        if (pictureField) {
            std::cout << "=== ИГРА ОКОНЧЕНА ===\n\n";
            std::cout << "Картинка: " << pictureField->getPictureName() << "\n";
            std::cout << "Игрок: " << playerName << "\n\n";
        }
    } else {
        std::cout << "=== ИГРА ОКОНЧЕНА ===\n\n";
        std::cout << "Игрок: " << playerName << std::endl;
        std::cout << "Итоговый счет: " << score << std::endl;
        std::cout << "Уровень: " << settings->getLevel() << std::endl;
        std::cout << "Очищено линий: " << linesClearedTotal << "\n\n";
    }
    if (score > 0) {
        scoreSystem.addScore(playerName, score);
    }
    scoreSystem.displayScores();
    
    std::cout << "\nНажмите любую клавишу для возврата в меню...";
    std::cout.flush();
    input.getInput();
    if (field) {
        delete field;
        field = nullptr;
    }
    
    score = 0;
    linesClearedTotal = 0;
    settings->setLevel(1);
    isPictureMode = false;
    gamePaused = false;
    TerminalHelper::clearScreen();
    if (GameMenu()) {
        gameRunning = true;
        if (isPictureMode) {
            view.ShowPictureField(*field);
        } else {
            view.ShowField(*field);
        }
        
        figure = FigureO();
        if (isPictureMode) {
            PictureField* pictureField = dynamic_cast<PictureField*>(field);
            if (pictureField) {
                pictureField->resetGame();
                int startX = (field->getWidth() - figure.getWidth()) / 2;
                figure.setPosition(startX, 1);
            }
        } else {
            figure.setPosition(10, 1);
        }
        
        showScore();
        showLevelInfo();
    } else {
        // Пользователь выбрал выход из игры
        gameRunning = false;
    }
}

void GameController::addPoints(int points) {
    score += points;
}

void GameController::updateLevel() {
    int currentLevel = settings->getLevel();
    int requiredLines = settings->getLinesForNextLevel();
    
    if (linesClearedTotal >= requiredLines) {
        settings->setLevel(currentLevel + 1);
        showLevelInfo();
        
        int messageY = field->getHeight() + 6;
        TerminalHelper::moveCursorTo(messageY, 0);
        TerminalHelper::clearCurrentLine();
        std::cout << "НОВЫЙ УРОВЕНЬ: " << settings->getLevel() << "!";
        usleep(1000000);
    }
}

void GameController::showLevelInfo() {
    if (!field) return;
    int startY = field->getHeight() + 4;
    int startX = 0;
    
    TerminalHelper::moveCursorTo(startY, startX);
    TerminalHelper::clearCurrentLine();
    if (isPictureMode) {
        PictureField* pictureField = (PictureField*)field;
        std::cout << "Режим: Собери картинку | Картинка: " << pictureField->getPictureName();
    } else {
        std::cout << "Уровень: " << settings->getLevel() 
                  << " | Линий до след. уровня: " 
                  << std::max(0, settings->getLinesForNextLevel() - linesClearedTotal)
                  << " | Дроп: +" << Settings::getDropPointsForLevel(settings->getLevel()) 
                  << " очков/клетка";
    }
    
    TerminalHelper::moveCursorToSafePosition();
    std::cout.flush();
}

void GameController::showScore() {
    if (!field) return;
    int startY = field->getHeight() + 2;
    int startX = 0;
    
    TerminalHelper::moveCursorTo(startY, startX);
    TerminalHelper::clearCurrentLine();
    if (isPictureMode) {
        PictureField* pictureField = dynamic_cast<PictureField*>(field);
        if (pictureField) {
            std::cout << "=== КАРТИНКА: " << pictureField->getPictureName() << " ===";
        } else {
            std::cout << "=== СОБЕРИ КАРТИНКУ ===";
        }
    } else {
        std::cout << "=== СТАТИСТИКА ===";
    }
    
    TerminalHelper::moveCursorTo(startY + 1, startX);
    TerminalHelper::clearCurrentLine();
    std::cout << "Игрок: " << playerName << " | Счет: " << score;
    TerminalHelper::moveCursorTo(startY + 2, startX);
    TerminalHelper::clearCurrentLine();
    
    char left = settings->getControl("LEFT");
    char right = settings->getControl("RIGHT");
    char down = settings->getControl("DOWN");
    char drop = settings->getControl("DROP");
    char rotate = settings->getControl("ROTATE");
    char pause = settings->getControl("PAUSE");
    char quit = settings->getControl("QUIT");
    std::string downStr = (down == ' ') ? "ПРОБЕЛ" : std::string(1, down);
    std::string dropStr = (drop == ' ') ? "ПРОБЕЛ" : std::string(1, drop);
    std::string rotateStr = (rotate == ' ') ? "ПРОБЕЛ" : std::string(1, rotate);
    
    std::cout << "Упр: " 
              << left << "/<-влево, "
              << right << "/->-вправо, "
              << down << "/v-вниз, "
              << drop << "/^-падение, "
              << rotate << "-поворот, "
              << pause << "-пауза, "
              << quit << "-выход";
    
    TerminalHelper::moveCursorToSafePosition();
    std::cout.flush();
}

bool GameController::GameMenu() {
    /**
     * @brief Отображает главное меню игры
     * @return true если пользователь выбрал игру, false для выхода
     * @note Позволяет выбрать режим игры, просмотреть рекорды, настроить управление
     */
    score = 0;
    linesClearedTotal = 0;
    settings->setLevel(1);
    isPictureMode = false;
    if (!nameEntered) {
        GetPlayerName();
    }
    TerminalHelper::clearScreen();
    std::cout << "            Добро пожаловать в Тетрис! " << std::endl;
    std::cout << "================================================" << std::endl;
    
    std::cout << "Текущее управление:" << std::endl;
    std::cout << "  Влево: " << settings->getControl("LEFT") 
              << "  Вправо: " << settings->getControl("RIGHT") 
              << "  Вниз: " << (settings->getControl("DOWN") == ' ' ? "ПРОБЕЛ" : std::string(1, settings->getControl("DOWN"))) 
              << std::endl;
    std::cout << "  Падение: " << (settings->getControl("DROP") == ' ' ? "ПРОБЕЛ" : std::string(1, settings->getControl("DROP"))) 
              << "  Поворот: " << (settings->getControl("ROTATE") == ' ' ? "ПРОБЕЛ" : std::string(1, settings->getControl("ROTATE"))) 
              << "  Пауза: " << settings->getControl("PAUSE") 
              << std::endl;
    std::cout << "================================================" << std::endl;
    std::cout << "\n";
    std::cout << "1. Выбрать игру" << std::endl;
    std::cout << "2. Таблица рекордов" << std::endl;
    std::cout << "3. Настройки" << std::endl;
    std::cout << "4. Выход" << std::endl;
    
    char c;
    do {
        c = input.getInput();
        if (c == '2') {
            TerminalHelper::clearScreen();
            scoreSystem.displayScores();
            std::cout << "\n1. Начать игру" << std::endl;
            std::cout << "2. Таблица рекордов" << std::endl;
            std::cout << "3. Настройки" << std::endl;
            std::cout << "4. Выход" << std::endl;
        }
        else if (c == '3') {
            ShowMainSettingsMenu();
            TerminalHelper::clearScreen();
            std::cout << "            Добро пожаловать в Тетрис! " << std::endl;
            std::cout << "================================================" << std::endl;
            std::cout << "Текущее управление:" << std::endl;
            std::cout << "  Влево: " << settings->getControl("LEFT") 
                      << "  Вправо: " << settings->getControl("RIGHT") 
                      << "  Вниз: " << (settings->getControl("DOWN") == ' ' ? "ПРОБЕЛ" : std::string(1, settings->getControl("DOWN"))) 
                      << std::endl;
            std::cout << "  Падение: " << (settings->getControl("DROP") == ' ' ? "ПРОБЕЛ" : std::string(1, settings->getControl("DROP"))) 
                      << "  Поворот: " << (settings->getControl("ROTATE") == ' ' ? "ПРОБЕЛ" : std::string(1, settings->getControl("ROTATE"))) 
                      << "  Пауза: " << settings->getControl("PAUSE") 
                      << std::endl;
            std::cout << "================================================" << std::endl;
            std::cout << "\n1. Начать игру" << std::endl;
            std::cout << "2. Таблица рекордов" << std::endl;
            std::cout << "3. Настройки" << std::endl;
            std::cout << "4. Выход" << std::endl;
        }
        else if (c == '4') {
            return false;
        }
        usleep(10000);
    } while (c != '1');
    
    TerminalHelper::clearScreen();
    std::cout << "Выберите игру:\n";
    std::cout << "1. Классический Тетрис\n";
    std::cout << "2. Ведро\n";
    std::cout << "3. Собери картинку\n";
    std::cout << "\nВыберите игру:\n ";
    
    char gameChoice;
    do {
        gameChoice = input.getInput();
        usleep(10000);
    } while (gameChoice < '1' || gameChoice > '3');
    
    if (gameChoice == '1') {
        field = new Field();
        isPictureMode = false;
        std::cout << "Выбрана классическая игра!" << std::endl;
        usleep(1000000);
        return true;
    } 
    else if (gameChoice == '2') {
        field = new BucketField();
        isPictureMode = false;
        std::cout << "Выбрана игра 'Ведро'!" << std::endl;
        usleep(1000000);
        return true;
    }
    else if (gameChoice == '3') {
        TerminalHelper::clearScreen();
        std::cout << "=== ВЫБЕРИТЕ КАРТИНКУ ===\n\n";
        std::cout << "1. Квадрат\n";
        std::cout << "2. Треугольник\n";
        //std::cout << "3. Домик\n";
       // std::cout << "4. Стрелка\n";
        //std::cout << "5. Бабочка\n";
        //std::cout << "6. Звезда\n";
        std::cout << "\nВыберите номер (1-2): ";
        std::cout.flush();
        
        char picChoice;
        do {
            picChoice = input.getInput();
            usleep(10000);
        } while (picChoice < '1' || picChoice > '2');
        
        int pictureType = PICTURE_SQUARE;
        switch(picChoice) {
            case '1': pictureType = PICTURE_SQUARE; break;
            case '2': pictureType = PICTURE_TRIANGLE; break;
            case '3': pictureType = PICTURE_HOUSE; break;
            case '4': pictureType = PICTURE_ARROW; break;
            case '5': pictureType = PICTURE_BUTTERFLY; break;
            case '6': pictureType = PICTURE_STAR; break;
        }
        
        if (field) {
            delete field;
            field = nullptr;
        }
        score = 0;
        linesClearedTotal = 0;
        count = 1;
        field = new PictureField(pictureType);
        isPictureMode = true;
        PictureField* pictureField = dynamic_cast<PictureField*>(field);
        if (pictureField) {
        pictureField->resetGame();}
        TerminalHelper::clearScreen();
        std::cout << "=== СОБЕРИ КАРТИНКУ ===\n\n";
        std::cout << "Задача: заполните серую область фигурами\n";
        std::cout << "Нажмите любую клавишу для начала...\n";
        std::cout << "Игра закончится, когда картинка будет собрана\n";
        std::cout << "или когда фигура выйдет за пределы серой области\n";
        input.getInput();

        view.ShowPictureField(*field);
        return true;
    }
    
    return false;
}

void GameController::run() {
    if (!TerminalHelper::isTerminalSizeValid(24, 48)) {
        int rows, cols;
        TerminalHelper::getCurrentSize(rows, cols);
        std::cerr << "Размер терминала слишком маленький! Минимум: 24 строки x 48 столбцов" << std::endl;
        std::cerr << "Текущий размер: " << cols << "x" << rows << std::endl;
        return;
    }
    
    TerminalHelper::enableAlternateBuffer();
    TerminalHelper::clearScreen();
    if (!nameEntered) {
        GetPlayerName();
    }
    if(GameMenu()) {
        if (isPictureMode) {
            view.ShowPictureField(*field);
        } else {
            view.ShowField(*field);
        }
        
        figure = FigureO();
        showScore();
        showLevelInfo();
        
        if (isPictureMode) {
            PictureField* pictureField = (PictureField*)field;
        }
        
        int messageY = field->getHeight() + 6;
        TerminalHelper::moveCursorTo(messageY, 0);
        TerminalHelper::clearCurrentLine();
        
        while (gameRunning) {
            if (TerminalHelper::wasResized()) {
                if (!TerminalHelper::isTerminalSizeValid(24, 48)) {
                    TerminalHelper::clearScreen();
                    int rows, cols;
                    TerminalHelper::getCurrentSize(rows, cols);
                    std::cout << "Размер терминала слишком маленький!" << std::endl;
                    std::cout << "Текущий размер: " << cols << "x" << rows << std::endl;
                    std::cout << "Требуется: 24 строки x 48 столбцов" << std::endl;
                    usleep(2000000);
                } else if (!gamePaused) {
                    if (isPictureMode) {
                        view.ShowPictureField(*field);
                    } else {
                        view.ShowField(*field);
                    }
                    view.ShowPlacedFigure(figure, *field);
                    showScore();
                    showLevelInfo();
                    if (isPictureMode) {
                        PictureField* pictureField = (PictureField*)field;
                    }
                }
            }
            
            if (!TerminalHelper::isTerminalSizeValid(24, 48)) {
                TerminalHelper::clearScreen();
                int rows, cols;
                TerminalHelper::getCurrentSize(rows, cols);
                usleep(100000);
                continue;
            }
            
            if (gamePaused) {
                Input();
                usleep(50000);
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

            int newX = figure.getstartx();
            int newY = figure.getstarty();

            view.ShowFigure(oldFigure, figure, *field, oldX, oldY, newX, newY);

            usleep(5000);
        }
    }
    TerminalHelper::disableAlternateBuffer();
    TerminalHelper::moveCursorToSafePosition();
}