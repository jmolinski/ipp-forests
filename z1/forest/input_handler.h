#include <stdbool.h>

#ifndef FOREST_INPUT_HANDLER_H
#define FOREST_INPUT_HANDLER_H

typedef struct Command_ {
    char** tokens;
    size_t size;
    bool error;
} Command;

void freeCommand(Command*);
Command *getNextCommand();

#endif //FOREST_INPUT_HANDLER_H
