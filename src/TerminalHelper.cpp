#include "TerminalHelper.h"

bool TerminalHelper::terminalResized = false;

void TerminalHelper::resizeHandler(int signo) {
    terminalResized = true;
}