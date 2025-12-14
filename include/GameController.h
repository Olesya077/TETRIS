#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "Field.h"
#include "ConsoleView.h"
#include "Figure.h"
#include "TerminalInput.h"
#include "Score.h"
#include "Settings.h"

/**
 * @brief Главный контроллер игры Тетрис
 * 
 * Управляет всем игровым процессом: обработкой ввода, логикой игры,
 * отображением, меню и взаимодействием с другими компонентами.
 */
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
    bool isPictureMode;
    bool nameEntered;
    int sequenceMode;          /**< Режим последовательности выпадающих фигур 
                                *   @note 0 - случайный выбор (по умолчанию)
                                *         1 - только фигуры типа O (квадрат)
                                *         2 - последовательное чередование фигур по порядку */
    int sequenceIndex;         /**< Индекс для чередования фигур */

public:
    GameController();
    ~GameController();
    
    /**
     * @brief Обрабатывает пользовательский ввод
     * @note Обрабатывает как игровые команды, так и меню
     */
    void Input();
    
    /**
     * @brief Обновляет позицию фигуры
     * @note Проверяет столкновения и создает новую фигуру при необходимости
     */
    void NewPosition();
    
    /**
     * @brief Главный игровой цикл
     * @note Управляет всей последовательностью игры
     */
    void run();
    
    /**
     * @brief Проверяет возможность перемещения фигуры
     * @param dx Смещение по X
     * @param dy Смещение по Y
     * @return true если перемещение возможно
     */
    bool CanMove(int dx, int dy);
    
    /**
     * @brief Обновляет отображение поля
     * @note Вызывает соответствующие методы отображения
     */
    void UpdateField();
    
    /**
     * @brief Автоматическое перемещение фигуры вниз
     * @note Вызывается по таймеру или в игровом цикле
     */
    void AutoMoveDown();
    
    /**
     * @brief Управление через терминал
     * @note Альтернативное название для Input()
     */
    void TerminalControl();
    
    /**
     * @brief Показывает меню паузы
     * @note Отображает опции продолжения, новой игры, настроек и выхода
     */
    void ShowPauseMenu();
    
    /**
     * @brief Показывает меню настроек
     * @param fromPause true если вызвано из меню паузы
     */
    void ShowSettingsMenu(bool fromPause = false);
    
    /**
     * @brief Показывает главное меню настроек
     * @note Отдельное меню для доступа из главного меню
     */
    void ShowMainSettingsMenu();
    
    /**
     * @brief Показывает настройки управления
     * @note Позволяет переназначить клавиши управления
     */
    void ShowControlSettings();
    
    /**
     * @brief Показывает текущие настройки
     */
    void ShowCurrentSettings();
    
    /**
     * @brief Добавляет очки к счету
     * @param points Количество добавляемых очков
     */
    void addPoints(int points);
    
    /**
     * @brief Показывает главное меню игры
     * @return true если игра должна начаться, false для выхода
     */
    bool GameMenu();
    
    /**
     * @brief Обновляет уровень игры
     * @note Повышает уровень при достижении нужного количества линий
     */
    void updateLevel();
    
    /**
     * @brief Показывает информацию о текущем уровне
     */
    void showLevelInfo();
    
    /**
     * @brief Запрашивает имя игрока
     * @note Используется при первом запуске игры
     */
    void GetPlayerName();
    
    /**
     * @brief Показывает экран завершения игры
     * @param isPictureModeGameOver true если игра в режиме "Собери картинку"
     */
    void showGameOverScreen(bool isPictureModeGameOver);
    /**
     * @brief Устанавливает режим последовательности выпадающих фигур
     * @param mode Режим последовательности:
     *             0 - случайные фигуры (стандартный режим)
     *             1 - только фигуры типа O (квадрат)
     *             2 - последовательное чередование фигур (O→L→T→I→S→Z→J→O...)
     * 
     * @note Вызывается из main() при обработке аргументов командной строки.
     */
    void setSequenceMode(int mode);
private:
    /**
     * @brief Быстрое падение фигуры (дроп)
     * @note Фигура падает до самого низа, игрок получает бонусные очки
     */
    void DropFigure();
    
    /**
     * @brief Проверяет возможность поворота фигуры
     * @return true если поворот возможен
     */
    bool CanRotate();
    
    /**
     * @brief Показывает текущий счет и управление
     */
    void showScore();
    
    /**
     * @brief Преобразует ввод пользователя
     * @param c Введенный символ
     * @return Преобразованный символ (для совместимости)
     * @deprecated Может быть удален в будущих версиях
     */
    char mapInput(char c);
    
    /**
     * @brief Возвращает в главное меню
     * @note Сбрасывает состояние игры и показывает меню
     */
    void returnToMenu();

};

#endif