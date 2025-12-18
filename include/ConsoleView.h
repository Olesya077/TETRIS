/**
 * @file ConsoleView.h
 * @brief Заголовочный файл, содержащий объявление класса ConsoleView
 * 
 * */
#ifndef CONSOLEVIEW_H
#define CONSOLEVIEW_H

#include "Field.h"
#include "PictureField.h"
#include <vector>
#include <string>

/**
 * @brief Класс для отображения игрового поля и фигур в консоли
 * 
 * Отвечает за всю графику в терминале, включая цветовое оформление,
 * отображение текущей фигуры, "призрачной" фигуры и специальных режимов.
 */
class ConsoleView {
public:
    /**
     * @brief Конструктор по умолчанию
     */
    ConsoleView();
    
    /**
     * @brief Отображает игровое поле
     * @param field Ссылка на игровое поле для отображения
     * @note Проверяет размер терминала перед отрисовкой
     */
    void ShowField(Field& field);
    
    /**
     * @brief Отображает перемещение фигуры
     * @param oldFigure Предыдущее состояние фигуры
     * @param figure Текущее состояние фигуры
     * @param field Игровое поле
     * @param oldX Предыдущая X-координата
     * @param oldY Предыдущая Y-координата
     * @param newX Новая X-координата
     * @param newY Новая Y-координата
     * @note Очищает старое положение и отрисовывает новое
     */
    void ShowFigure(Figure& oldFigure, Figure& figure, Field& field, 
                    int oldX, int oldY, int newX, int newY);
    
    /**
     * @brief Возвращает случайный цвет в формате ANSI
     * @return Строка с ANSI-кодом цвета
     */
    std::string getRandomColor();
    
    /**
     * @brief Отображает уже размещенную на поле фигуру
     * @param figure Фигура для отображения
     * @param field Игровое поле
     */
    void ShowPlacedFigure(Figure& figure, Field& field);
    
    /**
     * @brief Обновляет отображение после очистки линий
     * @param field Игровое поле
     * @param startRow Строка, с которой началась очистка
     */
    void UpdateClearedLines(Field& field, int startRow);
    
    /**
     * @brief Отображает "призрачную" фигуру (предпросмотр падения)
     * @param figure Текущая фигура
     * @param field Игровое поле
     * @note Показывает, куда упадет фигура при быстром падении
     */
    void ShowGhostFigure(Figure& figure, Field& field);
    
    /**
     * @brief Очищает "призрачную" фигуру
     * @param figure Фигура, для которой нужно очистить предпросмотр
     * @param field Игровое поле
     */
    void ClearGhostFigure(Figure& figure, Field& field);
    
    /**
     * @brief Отображает поле в режиме "Собери картинку"
     * @param field Ссылка на PictureField для отображения
     * @note Использует специальную отрисовку для целевых областей
     */
    void ShowPictureField(Field& field);

    void ShowPictureBackground(PictureField& pictureField);
};

#endif