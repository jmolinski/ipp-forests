#include "commands.h"
#include "output_interface.h"
#include "string.h"
#include <stdbool.h>

static inline bool areStringsEqual(char *s1, char *s2) {
    return strcmp(s1, s2) == 0;
}

static inline bool validatePrintArguments(Command *c) {
    return c->size >= 1 && c->size <= 3;
}

void executeCommandPrint(Command *c, Tree bst) {
    if (!validatePrintArguments(c)) {
        printError();
        return;
    }

    unsigned desiredNestLevel = c->size - 1;
    for (unsigned nestLevel = 0; nestLevel < desiredNestLevel; nestLevel++)
        bst = bstGet(bst, c->tokens[nestLevel + 1])->value;

    bstDisplaySorted(bst, printLine);
}

static inline bool validateAddArguments(Command *c) {
    return c->size >= 2 && c->size <= 4;
}

void executeCommandAdd(Command *c, Tree *bst) {
    if (!validateAddArguments(c)) {
        printError();
        return;
    }

    unsigned argumentsCount = c->size - 1;
    for (int i = 1; i <= argumentsCount; i++) {
        bst = &(bstGetOrCreate(bst, c->tokens[i]))->value;
    }

    printOk();
}

static inline bool validateDelArguments(Command *c) {
    return c->size >= 2 && c->size <= 4;
}

void executeCommandDel(Command *c, Tree bst) {
    if (!validateDelArguments(c)) {
        printError();
        return;
    }
}

static inline bool validateCheckArguments(Command *c) {
    bool correctArgumentsCount = c->size >= 2 && c->size <= 4;
    char *lastArgument = c->tokens[c->size - 1];
    bool lastArgumentNotAsterisk = (!areStringsEqual(lastArgument, "*"));
    return correctArgumentsCount && lastArgumentNotAsterisk;
}

void executeCommandCheck(Command *c, Tree bst) {
    if (!validateCheckArguments(c)) {
        printError();
        return;
    }
}

void executeCommandStrategy(Command *command, Tree *forests) {
    if (command->error) {
        printError();
        return;
    }

    char *commandName = command->tokens[0];

    if (areStringsEqual(commandName, "PRINT")) {
        executeCommandPrint(command, *forests);
    } else if (areStringsEqual(commandName, "ADD")) {
        executeCommandAdd(command, forests);
    } else if (areStringsEqual(commandName, "DEL")) {
        executeCommandDel(command, *forests);
    } else if (areStringsEqual(commandName, "CHECK")) {
        executeCommandCheck(command, *forests);
    } else {
        printError();
    }
}