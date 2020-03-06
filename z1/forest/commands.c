#include "commands.h"
#include "output_interface.h"
#include "string.h"
#include <stdbool.h>
#include <stdlib.h>

#define MAX_COMMAND_ARGUMENTS 6

static inline bool areStringsEqual(char *s1, char *s2) {
    return strcmp(s1, s2) == 0;
}

void executeCommandPrint(Command *c, Tree bst) {
    if (!(c->size >= 1 && c->size <= 3)) {
        printError();
        return;
    }

    unsigned desiredNestLevel = c->size - 1;
    for (unsigned nestLevel = 0; nestLevel < desiredNestLevel; nestLevel++) {
        bst = bstGet(bst, c->tokens[nestLevel + 1])->value;
    }

    bstDisplaySorted(bst, printLine);
}

void executeCommandAdd(Command *c, Tree *bst) {
    if (!(c->size >= 2 && c->size <= 4)) {
        printError();
        return;
    }

    unsigned argumentsCount = c->size - 1;
    for (int i = 1; i <= argumentsCount; i++) {
        bst = &(bstGetOrCreate(bst, c->tokens[i]))->value;
    }

    printOk();
}

void executeCommandDel(Command *c, Tree *bst) {
    if (!(c->size >= 2 && c->size <= 4)) {
        printError();
        return;
    }

    unsigned nestLevel = c->size - 2;
    for (int i = 0; i < nestLevel; i++) {
        bst = &(bstGet(*bst, c->tokens[i + 1]))->value;
    }

    if (bst != NULL) {
        bstDelete(*bst, c->tokens[c->size - 1]);
    }

    printOk();
}

static inline bool validateCheckArguments(Command *c) {
    bool correctArgumentsCount = c->size >= 2 && c->size <= 4;
    char *lastArgument = c->tokens[c->size - 1];
    bool lastArgumentNotAsterisk = (!areStringsEqual(lastArgument, "*"));
    return correctArgumentsCount && lastArgumentNotAsterisk;
}

bool bstForAny(Tree t, bool (*seekItem)(Tree, char **), char **tokensLeft) {
    return (t != NULL) && (bstForAny(t->left, seekItem, tokensLeft) ||
                           seekItem(t->value, tokensLeft + 1) ||
                           bstForAny(t->right, seekItem, tokensLeft));
}

bool recursiveSeekItem(Tree bst, char **tokensLeft) {
    if (*tokensLeft == NULL) {
        return bst != NULL;
    }

    char *token = *tokensLeft;
    if (areStringsEqual(token, "*")) {
        return bstForAny(bst, recursiveSeekItem, tokensLeft);
    } else {
        return recursiveSeekItem(bstGet(bst, token), tokensLeft + 1);
    }
}

void executeCommandCheck(Command *c, Tree bst) {
    if (!validateCheckArguments(c)) {
        printError();
        return;
    }

    char **argsList = malloc(sizeof(char *) * MAX_COMMAND_ARGUMENTS);
    for (int i = 0; i < c->size - 1; i++)
        argsList[i] = c->tokens[i + 1];

    argsList[c->size - 1] = 0; // iteration guard

    bool resultFound = recursiveSeekItem(bst, argsList);
    free(argsList);

    if (resultFound) {
        printYes();
    } else {
        printNo();
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
        executeCommandDel(command, forests);
    } else if (areStringsEqual(commandName, "CHECK")) {
        executeCommandCheck(command, *forests);
    } else {
        printError();
    }
}