/**
 * @file TerminalHelper.cpp
 * @brief Реализация статических методов TerminalHelper
 * 
 * Обработчик сигналов и управление флагами изменения размера терминала.
 * Содержит реализацию всех статических методов для работы с терминалом.
 */

#include "TerminalHelper.h"
#include <sys/ioctl.h>
#include <unistd.h>
#include <cstdio>
#include <iostream>
#include <csignal>
#include <cstring>

bool TerminalHelper::terminalResized = false;

void TerminalHelper::resizeHandler(int signo) {
    /**
     * @brief Обработчик сигнала SIGWINCH (изменение размера терминала)
     * @param signo Номер сигнала
     * @note Устанавливает флаг terminalResized в true
     *       Вызывается асинхронно при изменении размера окна
     */
    terminalResized = true;
}

bool TerminalHelper::getTerminalSize(int& rows, int& cols) {
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) {
        rows = w.ws_row;
        cols = w.ws_col;
        return true;
    }
    return false;
}

bool TerminalHelper::isTerminalSizeValid(int minRows, int minCols) {
    int rows, cols;
    if (getTerminalSize(rows, cols)) {
        return rows >= minRows && cols >= minCols;
    }
    return false;
}

void TerminalHelper::clearScreen() {
    printf("\033[2J\033[1;1H");
    fflush(stdout);
}

void TerminalHelper::moveCursorTo(int row, int col) {
    printf("\033[%d;%dH", row + 1, col + 1);
    fflush(stdout);
}

void TerminalHelper::moveCursorToSafePosition() {
    int rows, cols;
    if (getTerminalSize(rows, cols)) {
        printf("\033[%d;1H", rows);
        fflush(stdout);
    }
}

void TerminalHelper::initResizeHandler() {
    signal(SIGWINCH, resizeHandler);
}

bool TerminalHelper::wasResized() {
    bool resized = terminalResized;
    terminalResized = false;
    return resized;
}

void TerminalHelper::getCurrentSize(int& rows, int& cols) {
    getTerminalSize(rows, cols);
}

void TerminalHelper::saveScreen() {
    printf("\033[?1049h");
    fflush(stdout);
}

void TerminalHelper::restoreScreen() {
    printf("\033[?1049l");
    fflush(stdout);
}

void TerminalHelper::clearCurrentLine() {
    printf("\033[2K");
    fflush(stdout);
}

void TerminalHelper::enableAlternateBuffer() {
    printf("\033[?1049h\033[H");
    fflush(stdout);
}

void TerminalHelper::disableAlternateBuffer() {
    printf("\033[?1049l");
    fflush(stdout);
}

void TerminalHelper::disableScrolling() {
    printf("\033[?7l");
    printf("\033[r");
    fflush(stdout);
}

void TerminalHelper::enableScrolling() {
    printf("\033[?7h");
    fflush(stdout);
}

void TerminalHelper::setScrollRegion(int top, int bottom) {
    printf("\033[%d;%dr", top, bottom);
    fflush(stdout);
}

void TerminalHelper::clearScrollRegion() {
    printf("\033[r");
    fflush(stdout);
}

void TerminalHelper::saveCursor() {
    printf("\033[s");
    fflush(stdout);
}

void TerminalHelper::restoreCursor() {
    printf("\033[u");
    fflush(stdout);
}

void TerminalHelper::hideCursor() {
    printf("\033[?25l");
    fflush(stdout);
}

void TerminalHelper::showCursor() {
    printf("\033[?25h");
    fflush(stdout);
}