 /**
* @file Field.h
* @brief Заголовочный файл, содержащий объявление базового класса Field и производного класса BucketField
*/
#ifndef FIELD_H
#define FIELD_H

#include "Figure.h"
#include <vector>

/**
 * @brief Базовый класс игрового поля
 * 
 * Содержит логику размещения фигур, проверки столкновений и очистки линий.
 */
class Field {
protected:
    bool isPictureMode;                    /**< Флаг режима "Собери картинку" */
    std::vector<bool> fieldmatrix;        /**< Матрица занятых клеток */
    std::vector<std::string> fieldcolors; /**< Матрица цветов клеток */
    int fieldWidth = 22;                  /**< Ширина поля */
    int fieldHeight = 22;                 /**< Высота поля */

public:
    /**
     * @brief Конструктор по умолчанию
     * @note Инициализирует границы поля
     */
    Field();
    
    /**
     * @brief Виртуальный деструктор
     */
    virtual ~Field() = default;
    
    /**
     * @brief Размещает фигуру на поле
     * @param figure Фигура для размещения
     * @note Записывает фигуру в матрицу поля с сохранением цвета
     */
    virtual void placeFigure(Figure& figure);
    
    /**
     * @brief Проверяет, занята ли клетка
     * @param i Номер строки
     * @param j Номер столбца
     * @return true если клетка занята, false в противном случае
     */
    bool getch(int i, int j);
    
    /**
     * @brief Устанавливает состояние клетки
     * @param i Номер строки
     * @param j Номер столбца
     * @param value true - клетка занята, false - свободна
     * @param color Цвет клетки (по умолчанию пустая строка)
     */
    void setch(int i, int j, bool value, const std::string& color = "");
    
    /**
     * @brief Возвращает цвет клетки
     * @param i Номер строки
     * @param j Номер столбца
     * @return ANSI-строка с цветом или пустая строка
     */
    std::string getColor(int i, int j);
    
    /**
     * @brief Возвращает высоту поля
     * @return Высота поля в клетках
     */
    virtual int getHeight();
    
    /**
     * @brief Возвращает ширину поля
     * @return Ширина поля в клетках
     */
    virtual int getWidth();
    
    /**
     * @brief Возвращает ширину строки (для нестандартных полей)
     * @param row Номер строки
     * @return Ширина строки в клетках
     * @note В базовом классе всегда возвращает fieldWidth
     */
    virtual int getRowWidth(int row) { return fieldWidth; }
    
    /**
     * @brief Проверяет валидность позиции
     * @param row Номер строки
     * @param col Номер столбца
     * @return true если позиция внутри поля и не занята стенкой
     */
    virtual bool isValidPosition(int row, int col);
    
    /**
     * @brief Очищает заполненные линии
     * @return Количество очищенных линий
     * @note Сдвигает вышележащие линии вниз
     */
    virtual int clearFullLines();
};

/**
 * @brief Класс поля в форме ведра
 * 
 * Специализированное поле, сужающееся кверху (в форме ведра).
 * Переопределяет методы для работы с нестандартной геометрией.
 */
class BucketField : public Field {
private:
    std::vector<int> rowWidths; /**< Ширина каждой строки (для сужающегося поля) */

public:
    /**
     * @brief Конструктор поля-ведра
     * @note Инициализирует форму ведра
     */
    BucketField();
    
    /**
     * @brief Размещает фигуру с учетом формы ведра
     * @param figure Фигура для размещения
     * @override
     */
    virtual void placeFigure(Figure& figure) override;
    
    /**
     * @brief Возвращает ширину строки
     * @param row Номер строки
     * @return Ширина строки в клетках
     * @override
     */
    virtual int getRowWidth(int row) override;
    
    /**
     * @brief Возвращает общую ширину поля
     * @return Ширина поля в клетках
     * @override
     */
    virtual int getWidth() override { return fieldWidth; }
    
    /**
     * @brief Очищает заполненные линии с учетом формы ведра
     * @return Количество очищенных линий
     * @override
     */
    virtual int clearFullLines();
    
    /**
     * @brief Проверяет валидность позиции в ведре
     * @param row Номер строки
     * @param col Номер столбца
     * @return true если позиция внутри текущей ширины строки
     * @override
     */
    virtual bool isValidPosition(int row, int col);
    
private:
    /**
     * @brief Инициализирует форму ведра
     * @note Задает ширину каждой строки для создания сужающейся формы
     */
    void initializeBucketShape();
};

#endif