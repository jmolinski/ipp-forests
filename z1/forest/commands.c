#include "commands.h"
#include "output_interface.h"
#include "string.h"
#include <stdbool.h>

static inline bool areStringsEqual(char *s1, char *s2) {
    return strcmp(s1, s2) == 0;
}

static inline bool validatePrintArguments(Command *c) {
    return c->size == 1;
}

void executeCommandPrint(Command *c, Tree forests) {
    if(!validatePrintArguments(c)) {
        printError();
        return;
    }
}


static inline bool validateAddArguments(Command *c) {
    return c->size >= 2 && c->size <= 4;
}

void executeCommandAdd(Command *c, Tree forests) {
    if(!validateAddArguments(c)) {
        printError();
        return;
    }
}


static inline bool validateDelArguments(Command *c) {
    return c->size >= 2 && c->size <= 4;
}

void executeCommandDel(Command *c, Tree forests) {
    if(!validateDelArguments(c)) {
        printError();
        return;
    }
}


static inline bool validateCheckArguments(Command *c) {
    bool argumentsCorrect =
    bool rightCount = c->size >= 2 && c->size <= 4;

}

void executeCommandCheck(Command *c, Tree forests) {
}

void executeCommandStrategy(Command *command, Tree forests) {
    if (command->error) {
        printError();
        return;
    }

    char *commandName = command->tokens[0];
    if (areStringsEqual(commandName, "PRINT")) {
        executeCommandPrint(command, forests);
    } else if (areStringsEqual(commandName, "ADD")) {
        executeCommandAdd(command, forests);
    } else if (areStringsEqual(commandName, "DEL")) {
        executeCommandDel(command, forests);
    } else if (areStringsEqual(commandName, "CHECK")) {
        executeCommandCheck(command, forests);
    } else {
        printError();
    }
}