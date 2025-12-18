/**
 * @file TerminalHelper.h
 * @brief Заголовочный файл, содержащий объявление класса TerminalHelper для работы с терминалом
 */
#ifndef TERMINALHELPER_H
#define TERMINALHELPER_H

#include <sys/ioctl.h>
#include <unistd.h>
#include <cstdio>
#include <thread>
#include <chrono>
#include <iostream>
#include <csignal>

/**
 * @brief Вспомогательный класс для работы с терминалом
 * 
 * Предоставляет статические методы для управления терминалом:
 * - Определение размеров терминала
 * - Управление курсором и экраном
 * - Обработка сигналов изменения размера
 * - Управление буферами экрана
 */
class TerminalHelper {
private:
    static bool terminalResized; /**< Флаг изменения размера терминала */
    
    /**
     * @brief Обработчик сигнала изменения размера терминала
     * @param signo Номер сигнала (должен быть SIGWINCH)
     */
    static void resizeHandler(int signo);

public:
    /**
     * @brief Получает текущий размер терминала
     * @return true если размер успешно получен
     */
    static bool getTerminalSize(int& rows, int& cols);
    
    /**
     * @brief Проверяет, достаточно ли размер терминала
     * @param minRows Минимальное количество строк (по умолчанию 24)
     * @param minCols Минимальное количество столбцов (по умолчанию 48)
     * @return true если терминал достаточно большой
     */
    static bool isTerminalSizeValid(int minRows = 24, int minCols = 48);
    
    /**
     * @brief Очищает экран терминала
     * @note Использует ANSI escape-последовательности
     */
    static void clearScreen();
    
    /**
     * @brief Перемещает курсор в указанную позицию
     */
    static void moveCursorTo(int row, int col);
    
    /**
     * @brief Перемещает курсор в безопасную позицию (низ экрана)
     */
    static void moveCursorToSafePosition();
    
    /**
     * @brief Инициализирует обработчик изменения размера терминала
     * @note Устанавливает обработчик для сигнала SIGWINCH
     */
    static void initResizeHandler();

    /**
     * @brief Проверяет, изменился ли размер терминала
     * @return true если размер изменился с последней проверки
     */
    static bool wasResized();

    /**
     * @brief Получает текущий размер терминала
     */
    static void getCurrentSize(int& rows, int& cols);
    
    /**
     * @brief Сохраняет текущий экран (включает альтернативный буфер)
     */
    static void saveScreen();
    
    /**
     * @brief Восстанавливает сохраненный экран (выключает альтернативный буфер)
     */
    static void restoreScreen();
    
    /**
     * @brief Очищает текущую строку
     */
    static void clearCurrentLine();
    
    /**
     * @brief Включает альтернативный буфер экрана
     */
    static void enableAlternateBuffer();
    
    /**
     * @brief Выключает альтернативный буфер экрана
     */
    static void disableAlternateBuffer();
    
    /**
     * @brief Отключает прокрутку терминала
     */
    static void disableScrolling();
    
    /**
     * @brief Включает прокрутку терминала
     */
    static void enableScrolling();
    
    /**
     * @brief Устанавливает область прокрутки
     * @param top Верхняя граница области
     * @param bottom Нижняя граница области
     */
    static void setScrollRegion(int top, int bottom);
    
    /**
     * @brief Сбрасывает область прокрутки на весь экран
     */
    static void clearScrollRegion();
    
    /**
     * @brief Сохраняет текущую позицию курсора
     */
    static void saveCursor();
    
    /**
     * @brief Восстанавливает сохраненную позицию курсора
     */
    static void restoreCursor();
    
    /**
     * @brief Скрывает курсор
     */
    static void hideCursor();
    
    /**
     * @brief Показывает курсор
     */
    static void showCursor();
};

#endif