#include <stdbool.h>

#ifndef FOREST_INPUT_HANDLER_H
#define FOREST_INPUT_HANDLER_H

typedef struct Command_ {
    char **tokens;
    unsigned size;
    bool error;
} Command;

void freeCommand(Command *);
char* copyString(char*);
Command *getNextCommand();

#endif // FOREST_INPUT_HANDLER_H
