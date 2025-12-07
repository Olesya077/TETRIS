#include "GameController.h"
#include "TerminalInput.h"
#include "TerminalHelper.h"
#include <iostream>
#include <unistd.h>
#include <algorithm>
#include <cctype>

GameController::GameController() : field(nullptr), gameRunning(true), 
                        count(1), gamePaused(false), score(0), linesClearedTotal(0), playerName("Player") {
    figure.setPosition(10, 1);
    TerminalHelper::initResizeHandler();
    settings = Settings::getInstance();
}

GameController::~GameController() {
    delete field;
    field = nullptr;
    Settings::destroyInstance();
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
    char c = input.getInput();
    
    if (gamePaused) {
        switch(c) {
            case 'r':
                gamePaused = false;
                TerminalHelper::restoreScreen();
                view.ShowField(*field);
                view.ShowPlacedFigure(figure, *field);
                showScore();
                showLevelInfo();
                break;
            case 'q':
                TerminalHelper::restoreScreen();
                gameRunning = false;
                break;
            case 'n':
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
    
    if (c == settings->getControl("LEFT")) {
        if (CanMove(-1, 0)) {
            figure.setPosition(figure.getstartx() - 1, figure.getstarty());
        }
    }
    else if (c == settings->getControl("RIGHT")) {
        if (CanMove(1, 0)) {
            figure.setPosition(figure.getstartx() + 1, figure.getstarty());
        }
    }
    else if (c == settings->getControl("DOWN")) {
        if (CanMove(0, 1)) {
            figure.setPosition(figure.getstartx(), figure.getstarty() + 1);
            addPoints(40 * settings->getLevel());
            showScore();
        }
    }
    else if (c == settings->getControl("DROP")) {
        DropFigure();
    }
    else if (c == settings->getControl("ROTATE")) {
        if (CanRotate()) {
            figure.rotate();
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
    TerminalHelper::saveScreen();
    TerminalHelper::clearScreen();
    std::cout << "\033[s";
    std::cout << "=== ПАУЗА ===" << std::endl;
    std::cout << "Игрок: " << playerName << " | Текущий счет: " << score << std::endl;
    std::cout << "Уровень: " << settings->getLevel() << " | Линий очищено: " << linesClearedTotal << std::endl;
    std::cout << "----------------------------" << std::endl;
    std::cout << "Нажмите r чтобы вернуться в игру" << std::endl;
    std::cout << "Нажмите n чтобы начать новую игру" << std::endl;
    std::cout << "Нажмите s чтобы открыть настройки" << std::endl;
    std::cout << "Нажмите v чтобы посмотреть рекорды" << std::endl;
    std::cout << "Нажмите q чтобы выйти" << std::endl;
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
    do {
        choice = input.getInput();
        
        if (choice == '1') {
            ShowControlSettings();
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
        
        usleep(10000);
    } while (true);
}

void GameController::ShowControlSettings() {
    TerminalHelper::clearScreen();
    std::cout << "=== ИЗМЕНЕНИЕ УПРАВЛЕНИЯ ===" << std::endl;
    std::cout << "----------------------------" << std::endl;
    
    auto actions = settings->getAvailableActions();
    auto occupiedKeys = settings->getOccupiedKeys();
    
    std::cout << "Доступные действия:" << std::endl;
    for (size_t i = 0; i < actions.size(); i++) {
        char key = settings->getControl(actions[i]);
        std::string keyStr = (key == ' ') ? "ПРОБЕЛ" : std::string(1, key);
        std::cout << (i+1) << ". " << actions[i] << " (" << keyStr << ")" << std::endl;
    }
    
    std::cout << "\nЗанятые клавиши: ";
    for (char key : occupiedKeys) {
        if (key != ' ') {
            std::cout << key << " ";
        }
    }
    std::cout << "\n*Системные клавиши (нельзя изменить): 1, 2, 3, 4, s, h, r, n, v, q" << std::endl;
    
    std::cout << "\nВведите номер действия для изменения (или 0 для отмены): ";
    
    char actionChoice;
    do {
        actionChoice = input.getInput();
        if (actionChoice == '0') {
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
            
            std::cout << "Введите новую клавишу: ";
            
            char newKey;
            do {
                newKey = input.getInput();
                if (newKey != 0) {
                    break;
                }
                usleep(10000);
            } while (true);
            
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
    } while (true);
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
    do {
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
            do {
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
                }
                usleep(10000);
            } while (true);
        }
        else if (choice == '4') {
            return;
        }
        
        usleep(10000);
    } while (true);
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
    if (!field) return;
        
    if (CanMove(0, 1)) {
    } else {
        field->placeFigure(figure);
        view.ShowPlacedFigure(figure, *field);
        
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
        }
        
        int figureType = count % 7;
        switch(figureType) {
            case 0: figure = FigureO(); break;
            case 1: figure = FigureL(); break;
            case 2: figure = FigureT(); break;
            case 3: figure = FigureI(); break;
            case 4: figure = FigureS(); break;
            case 5: figure = FigureZ(); break;
            case 6: figure = FigureJ(); break;
        }
        count++;
        
        figure.setPosition(10, 1);
        
        if (!CanMove(0, 0)) {
            gameRunning = false;
            TerminalHelper::moveCursorTo(field->getHeight() + 3, 0);
            TerminalHelper::clearCurrentLine();
            std::cout << "GAME OVER! Итоговый счет: " << score 
                      << " | Уровень: " << settings->getLevel() << std::endl;

            scoreSystem.addScore(playerName, score);
            usleep(3000000);
        }
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
    std::cout << "Уровень: " << settings->getLevel() 
              << " | Линий до след. уровня: " 
              << std::max(0, settings->getLinesForNextLevel() - linesClearedTotal)
              << " | Дроп: +" << Settings::getDropPointsForLevel(settings->getLevel()) 
              << " очков/клетка";
    
    TerminalHelper::moveCursorToSafePosition();
    std::cout.flush();
}

void GameController::showScore() {
    if (!field) return;
    int startY = field->getHeight() + 2;
    int startX = 0;
    
    TerminalHelper::moveCursorTo(startY, startX);
    TerminalHelper::clearCurrentLine();
    std::cout << "=== СТАТИСТИКА ===";
    
    TerminalHelper::moveCursorTo(startY + 1, startX);
    TerminalHelper::clearCurrentLine();
    std::cout << "Игрок: " << playerName << " | Счет: " << score << " | Линий: " << linesClearedTotal;
    TerminalHelper::moveCursorTo(startY + 2, startX);
    TerminalHelper::clearCurrentLine();
    std::cout << "Управление: " 
              << settings->getControl("LEFT") << "/" 
              << settings->getControl("RIGHT") << "-влево/вправо, "
              << (settings->getControl("DOWN") == ' ' ? "ПРОБЕЛ" : std::string(1, settings->getControl("DOWN"))) << "-вниз, "
              << (settings->getControl("DROP") == ' ' ? "ПРОБЕЛ" : std::string(1, settings->getControl("DROP"))) << "-падение, "
              << (settings->getControl("ROTATE") == ' ' ? "ПРОБЕЛ" : std::string(1, settings->getControl("ROTATE"))) << "-поворот, "
              << settings->getControl("PAUSE") << "-пауза, "
              << settings->getControl("QUIT") << "-выход";

    showLevelInfo();
    TerminalHelper::moveCursorToSafePosition();
    std::cout.flush();
}

bool GameController::GameMenu() {
    score = 0;
    linesClearedTotal = 0;
    settings->setLevel(1);

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

    scoreSystem.displayScores();
    std::cout << "\n";
 
    std::cout << "Введите ваше имя: ";
    std::cin >> playerName;
    std::cin.ignore();
    std::cout << "\n";
    
    std::cout << "1. Начать игру" << std::endl;
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
    std::cout << "Выберите тип поля:" << std::endl;
    std::cout << "s - стандартное поле" << std::endl;
    std::cout << "h - расширяющееся поле (ведро)" << std::endl;
    
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
    if (!TerminalHelper::isTerminalSizeValid(24, 48)) {
        int rows, cols;
        TerminalHelper::getCurrentSize(rows, cols);
        std::cerr << "Размер терминала слишком маленький! Минимум: 24 строки x 48 столбцов" << std::endl;
        std::cerr << "Текущий размер: " << cols << "x" << rows << std::endl;
        return;
    }
    
    TerminalHelper::clearScreen();
    if(GameMenu()) {
        view.ShowField(*field);
        figure = FigureO();
        showScore();
        showLevelInfo();
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
                    view.ShowField(*field);
                    view.ShowPlacedFigure(figure, *field);
                    showScore();
                    showLevelInfo();
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
            
            int levelSpeed = Settings::getSpeedForLevel(settings->getLevel());
            usleep(levelSpeed);
            
            int newX = figure.getstartx();
            int newY = figure.getstarty();

            view.ShowFigure(oldFigure, figure, *field, oldX, oldY, newX, newY);
        }
    }
    TerminalHelper::restoreScreen();
    TerminalHelper::moveCursorToSafePosition();
}

void GameController::UpdateField() {}
void GameController::TerminalControl() {}