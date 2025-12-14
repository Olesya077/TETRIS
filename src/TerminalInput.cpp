/**
 * @file TerminalInput.cpp
 * @brief Реализация неблокирующего ввода с терминала
 * 
 * Настраивает raw-режим терминала для чтения одиночных символов.
 */
#include "TerminalInput.h"
#include <unistd.h>
#include <termios.h>
#include <stdio.h>

TerminalInput::TerminalInput() {
    /**
     * @brief Конструктор - настраивает терминал
     * @note Отключает:
     *       - Канонический режим (ожидание Enter)
     *       - Эхо ввода
     *       Устанавливает:
     *       - Чтение по 1 символу
     *       - Без таймаута
     */
    struct termios term;
    tcgetattr(STDIN_FILENO, &originalTerm);
    term = originalTerm;
    term.c_lflag &= ~(ICANON | ECHO);
    term.c_cc[VMIN] = 1;
    term.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

TerminalInput::~TerminalInput() {
    tcsetattr(STDIN_FILENO, TCSANOW, &originalTerm);
}

char TerminalInput::getInput() {
    /**
     * @brief Читает один символ из терминала
     * @return Прочитанный символ или 0 при ошибке
     * @note Использует системный вызов read()
     *       Не блокирует выполнение если нет данных
     */
    char c = 0;
    if (read(STDIN_FILENO, &c, 1) != 1) {
        return 0;
    }
    return c;
}

char TerminalInput::getInputWithArrows() {
    char c = getInput();
    if (c == 27) {
        char next = getInput();
        if (next == '[') { 
            char arrow = getInput();
            switch (arrow) {
                case 'A': return '^';
                case 'B': return 'v';
                case 'C': return '>';
                case 'D': return '<';
            }
        }
    }
    
    return c;
}       