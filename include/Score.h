#ifndef SCORESYSTEM_H
#define SCORESYSTEM_H

#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

/**
 * @brief Структура, представляющая запись о рекорде
 * 
 * Содержит информацию об игроке, его счете и дате установки рекорда.
 */
struct HighScore {
    std::string playerName;
    int score;
    std::string date;
};

/**
 * @brief Класс для управления рекордами игры
 * 
 * Обеспечивает загрузку, сохранение, добавление и отображение рекордов.
 * Рекорды хранятся в файле и сортируются по убыванию счета.
 */
class GameScore {
private:
    std::vector<HighScore> highScores;
    std::string scoreFile = "tetris_scores.txt";
    
public:
    /**
     * @brief Конструктор по умолчанию
     * @note Автоматически загружает рекорды из файла при создании
     */
    GameScore();
    
    /**
     * @brief Загружает рекорды из файла
     * @warning Если файл не существует, создается пустой список рекордов
     */
    void loadScores();
    
    /**
     * @brief Сохраняет рекорды в файл
     * @note Сохраняет только топ-10 рекордов
     */
    void saveScores();
    
    /**
     * @brief Добавляет новый рекорд
     * @note Автоматически добавляет текущую дату и сортирует список
     */
    void addScore(const std::string& name, int score);
    
    /**
     * @brief Отображает таблицу рекордов в консоли
     * @note Выводит топ-10 рекордов в форматированной таблице
     */
    void displayScores() const;
    
    /**
     * @brief Возвращает указанное количество лучших рекордов
     * @param count Количество возвращаемых рекордов (по умолчанию 10)
     * @return Вектор с лучшими рекордами, отсортированный по убыванию счета
     */
    std::vector<HighScore> getTopScores(int count = 10) const;
};

#endif