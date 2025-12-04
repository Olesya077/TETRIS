#include "TerminalInput.h"
#include <termios.h>
#include <unistd.h>
#include <stdio.h>

TerminalInput::TerminalInput() {
tcgetattr(STDIN_FILENO, &originalTerm);
struct termios term;
tcgetattr(STDIN_FILENO, &term);
term.c_lflag &= ~(ICANON | ECHO);
term.c_cc[VMIN] = 0;
term.c_cc[VTIME] = 0;
tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

TerminalInput::~TerminalInput() {

    tcsetattr(STDIN_FILENO, TCSANOW, &originalTerm);
}

char TerminalInput::getInput() {
char c = 0;
if (read(STDIN_FILENO, &c, 1) == 1) {
return c;
}
return 0;
}
