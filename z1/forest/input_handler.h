#include <stdbool.h>

#ifndef FOREST_INPUT_HANDLER_H
#define FOREST_INPUT_HANDLER_H

typedef struct Command_ {
    char **tokens;
    unsigned size;
    bool error;
} Command;

/* frees Command struct */
void freeCommand(Command *);

/* returns a pointer to a new copy of a string */
char* copyString(char*);

/* returns the next parsed command from stdin
 * ignores empty and comment lines
 * returns NULL if there are no more characters left on stdin */
Command *getNextCommand();

#endif // FOREST_INPUT_HANDLER_H
