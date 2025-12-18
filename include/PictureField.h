/**
 * @file PictureField.h
 * @brief Заголовочный файл, содержащий объявление класса PictureField для режима "Собери картинку"
 */
#ifndef PICTUREFIELD_H
#define PICTUREFIELD_H

#include "Field.h"
#include <iostream>
#include <vector>
#include <string>

/**
 * @brief Константы типов картинок
 * @note Используются для идентификации различных картинок в режиме сборки
 */
const int PICTURE_SQUARE = 1;
const int PICTURE_DIAMOND = 2;
const int PICTURE_HOUSE = 3;
const int PICTURE_ARROW = 4;
const int PICTURE_BUTTERFLY = 5;
const int PICTURE_STAR = 6;
const int PICTURE_TRIANGLE = 7;

/**
 * @brief Класс поля для режима "Собери картинку"
 * 
 * Специализированное поле с целевой областью, которую нужно заполнить фигурами.
 * Игрок должен заполнить заданную форму, не выходя за ее границы.
 */
class PictureField : public Field {
private:
    std::vector<bool> targetPicture;
    std::vector<bool> currentPicture;
    std::vector<std::string> pictureColors;
    int currentPictureType;
    bool gameOver;
    
    /**
     * @brief Загружает картинку указанного типа
     */
    void loadPicture(int type);
    
    /**
     * @brief Создает картинку "Квадрат"
     */
    void drawSquare();
    
    /**
     * @brief Создает картинку "Ромб"
     */
    void drawDiamond();
    
    /**
     * @brief Создает картинку "Домик"
     */
    void drawHouse();
    
    /**
     * @brief Создает картинку "Стрелка"
     */
    void drawArrow();
    
    /**
     * @brief Создает картинку "Бабочка"
     */
    void drawButterfly();
    
    /**
     * @brief Создает картинку "Звезда"
     */
    void drawStar();
    
    /**
     * @brief Создает картинку "Треугольник"
     */
    void drawTriangle();
    
public:
    /**
     * @brief Конструктор по умолчанию
     * @note Создает картинку-квадрат по умолчанию
     */
    PictureField();
    
    /**
     * @brief Конструктор с выбором типа картинки
     * @param type Тип создаваемой картинки
     */
    PictureField(int type);
    
    /**
     * @brief Размещает фигуру на поле с проверкой границ картинки
     * @note Завершает игру при выходе за границы целевой области
     */
    virtual void placeFigure(Figure& figure) override;
    
    /**
     * @brief Проверяет валидность позиции на поле
     * @return true если позиция внутри поля
     * @override
     */
    virtual bool isValidPosition(int row, int col) override;
    
    /**
     * @brief Проверяет, находится ли позиция в целевой области
     * @return true если позиция в целевой области картинки
     */
    bool isInTargetArea(int row, int col) const;
    
    /**
     * @brief Проверяет, полностью ли собрана картинка
     * @return true если все клетки целевой области заполнены
     */
    bool isPictureComplete() const;
    
    /**
     * @brief Показывает прогресс сборки картинки
     */
    void showProgress() const;
    
    /**
     * @brief Возвращает имя текущей картинки
     * @return Строка с именем картинки
     */
    std::string getPictureName() const;
    
    /**
     * @brief Проверяет, завершена ли игра
     * @return true если игра завершена
     */
    bool isGameOver() const { return gameOver; }
    
    /**
     * @brief Устанавливает состояние завершения игры
     * @param over Новое состояние
     */
    void setGameOver(bool over) { gameOver = over; }
    
    /**
     * @brief Сбрасывает состояние игры
     * @note Очищает текущее заполнение картинки
     */
    void resetGame();
    
    /**
     * @brief Возвращает границы картинки
     * @param minRow Минимальная строка (выходной параметр)
     * @param maxRow Максимальная строка (выходной параметр)
     * @param minCol Минимальный столбец (выходной параметр)
     * @param maxCol Максимальный столбец (выходной параметр)
     */
    void getPictureBounds(int& minRow, int& maxRow, 
                          int& minCol, int& maxCol) const;
    
    /**
     * @brief Показывает фон в указанной позиции
     * @param row Номер строки
     * @param col Номер столбца
     */
    void showBackgroundAt(int row, int col) const;
    
};

#endif