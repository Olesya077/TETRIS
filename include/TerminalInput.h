#ifndef INPUT_H
#define INPUT_H
#include <termios.h>
class TerminalInput {
private:
    struct termios originalTerm;
public:
    TerminalInput();
    ~TerminalInput();
    char getInput();
};

#endif