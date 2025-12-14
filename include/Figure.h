#ifndef FIGURE_H
#define FIGURE_H

#include <vector>
#include <string>
using namespace std;

/**
 * @brief Базовый класс фигуры Тетриса
 * 
 * Представляет абстрактную фигуру с возможностью вращения и перемещения.
 * Содержит информацию о форме, цвете, положении и состоянии вращения.
 */
class Figure {
protected:
    std::vector<bool> matrix; /**< Матрица формы фигуры (true - клетка занята) */
    string color;             /**< ANSI-строка с цветом фигуры */
    int width = 2;            /**< Ширина фигуры в клетках */           
    int height = 2;           /**< Высота фигуры в клетках */
    int startx = 10;          /**< X-координата левого верхнего угла */
    int starty = 1;           /**< Y-координата левого верхнего угла */
    int rotationState = 0;    /**< Текущее состояние вращения (0-3) */

public:
    /**
     * @brief Конструктор по умолчанию
     * @note Создает фигуру 2x2
     */
    Figure();
    
    /**
     * @brief Возвращает константную ссылку на матрицу формы
     * @return Константная ссылка на вектор bool
     */
    const std::vector<bool>& getMatrix() const { return matrix; }
    
    /**
     * @brief Устанавливает новую матрицу формы
     * @param newMatrix Новая матрица формы
     * @param newWidth Новая ширина
     * @param newHeight Новая высота
     */
    void setMatrix(const std::vector<bool>& newMatrix, int newWidth, int newHeight) {
        matrix = newMatrix;
        width = newWidth;
        height = newHeight;
    }
    
    /**
     * @brief Проверяет, занята ли клетка в фигуре
     * @param i Номер строки в матрице фигуры
     * @param j Номер столбца в матрице фигуры
     * @return true если клетка занята
     */
    bool getchar(int i, int j);
    
    /**
     * @brief Возвращает X-координату фигуры
     * @return Координата X
     */
    int getstartx();
    
    /**
     * @brief Возвращает Y-координату фигуры
     * @return Координата Y
     */
    int getstarty();
    
    /**
     * @brief Возвращает высоту фигуры
     * @return Высота в клетках
     */
    int getHeight();
    
    /**
     * @brief Возвращает ширину фигуры
     * @return Ширина в клетках
     */
    int getWidth();
    
    /**
     * @brief Возвращает цвет фигуры
     * @return ANSI-строка с цветом
     */
    string getcolor();
    
    /**
     * @brief Возвращает текущее состояние вращения
     * @return Число от 0 до 3
     */
    int getRotationState() const { return rotationState; };

    /**
     * @brief Устанавливает позицию фигуры на поле
     * @param x Новая X-координата
     * @param y Новая Y-координата
     */
    void setPosition(int x, int y);
    
    /**
     * @brief Вращает фигуру на 90 градусов по часовой стрелке
     * @note Для каждой фигуры определены свои матрицы вращения
     */
    void rotate();
};

/**
 * @brief Класс фигуры O (квадрат)
 * 
 * Фигура в форме квадрата 2x2, не вращается.
 */
class FigureO : public Figure {
public:
    /**
     * @brief Конструктор фигуры O
     * @note Устанавливает фиолетовый цвет
     */
    FigureO();
};

/**
 * @brief Класс фигуры L
 * 
 * Фигура в форме буквы L, имеет 4 состояния вращения.
 */
class FigureL : public Figure {
public:
    /**
     * @brief Конструктор фигуры L
     * @note Устанавливает голубой цвет
     */
    FigureL();
};

/**
 * @brief Класс фигуры T
 * 
 * Фигура в форме буквы T, имеет 4 состояния вращения.
 */
class FigureT : public Figure {
public:
    /**
     * @brief Конструктор фигуры T
     * @note Устанавливает желтый цвет
     */
    FigureT();
};

/**
 * @brief Класс фигуры I (палка)
 * 
 * Фигура в форме палки 4x1, имеет 2 состояния вращения.
 */
class FigureI : public Figure {
public:
    /**
     * @brief Конструктор фигуры I
     * @note Устанавливает синий цвет
     */
    FigureI();
};

/**
 * @brief Класс фигуры S
 * 
 * Фигура в форме буквы S, имеет 4 состояния вращения.
 */
class FigureS : public Figure {
public:
    /**
     * @brief Конструктор фигуры S
     * @note Устанавливает зеленый цвет
     */
    FigureS();
};

/**
 * @brief Класс фигуры Z
 * 
 * Фигура в форме буквы Z, имеет 4 состояния вращения.
 */
class FigureZ : public Figure {
public:
    /**
     * @brief Конструктор фигуры Z
     * @note Устанавливает красный цвет
     */
    FigureZ();
};

/**
 * @brief Класс фигуры J
 * 
 * Фигура в форме буквы J, имеет 4 состояния вращения.
 */
class FigureJ : public Figure {
public:
    /**
     * @brief Конструктор фигуры J
     * @note Устанавливает белый цвет
     */
    FigureJ();
};

#endif