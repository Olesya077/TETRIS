#ifndef TERMINALHELPER_H
#define TERMINALHELPER_H

#include <sys/ioctl.h>
#include <unistd.h>
#include <cstdio>
#include <thread>
#include <chrono>
#include <iostream>
#include <csignal>

class TerminalHelper {
private:
    static bool terminalResized;
    static void resizeHandler(int signo);

public:
    static bool getTerminalSize(int& rows, int& cols) {
        struct winsize w;
        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) {
            rows = w.ws_row;
            cols = w.ws_col;
            return true;
        }
        return false;
    }
    
    static bool isTerminalSizeValid(int minRows = 24, int minCols = 24) {
        int rows, cols;
        if (getTerminalSize(rows, cols)) {
            return rows >= minRows && cols >= minCols;
        }
        return false;
    }
    
    static void clearScreen() {
        printf("\033[2J\033[1;1H");
        fflush(stdout);
    }
    
    static void moveCursorTo(int row, int col) {
        std::cout << "\033[" << (row + 1) << ";" << (col + 1) << "H";
    }
    
    static void moveCursorToSafePosition() {
        int rows, cols;
        if (getTerminalSize(rows, cols)) {
            printf("\033[%d;1H", rows);
            fflush(stdout);
        }
    }
    static void initResizeHandler() {
        signal(SIGWINCH, resizeHandler);
    }

    static bool wasResized() {
        bool resized = terminalResized;
        terminalResized = false;
        return resized;
    }

    static void getCurrentSize(int& rows, int& cols) {
        getTerminalSize(rows, cols);
    }
    static void saveScreen() {
        printf("\033[?1049h");
        fflush(stdout);
    }
    
    static void restoreScreen() {
        printf("\033[?1049l");
        fflush(stdout);
    }
    
    static void clearCurrentLine() {
        printf("\033[2K");
        fflush(stdout);
    }
};

#endif