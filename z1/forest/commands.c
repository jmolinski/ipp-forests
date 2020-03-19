#include "commands.h"
#include "output_interface.h"
#include "safe_malloc.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/*
 * the specification of supported commands can be found here:
 * https://moodle.mimuw.edu.pl/mod/assign/view.php?id=21480
 */

// it's an upper bound, not the supremum
#define MAX_COMMAND_ARGUMENTS 6

static inline bool areStringsEqual(char *s1, char *s2) {
    return strcmp(s1, s2) == 0;
}

static void executeCommandPrint(Command *c, Tree bst) {
    if (!(c->size >= 1 && c->size <= 3)) {
        printError();
        return;
    }

    unsigned desiredNestLevel = c->size - 1;
    for (unsigned nestLevel = 1; nestLevel <= desiredNestLevel; nestLevel++) {
        bst = bstGet(bst, c->tokens[nestLevel]);
        if (bst == NULL) {
            return;
        }
        bst = bst->value;
    }

    bstDisplaySorted(bst, printLine);
}

static void executeCommandAdd(Command *c, Tree *bst) {
    if (!(c->size >= 2 && c->size <= 4)) {
        printError();
        return;
    }

    unsigned argumentsCount = c->size - 1;
    for (unsigned i = 1; i <= argumentsCount; i++) {
        bst = &(bstGetOrCreate(bst, c->tokens[i]))->value;
    }

    printOk();
}

static void executeCommandDel(Command *c, Tree *bst) {
    if (!(c->size >= 1 && c->size <= 4)) {
        printError();
        return;
    }

    if (c->size == 1) {
        freeTree(*bst);
        *bst = NULL;
        printOk();
        return;
    }

    unsigned desiredNestLevel = c->size - 2;
    for (unsigned i = 1; i <= desiredNestLevel && bst != NULL; i++) {
        Tree node = bstGet(*bst, c->tokens[i]);
        bst = node == NULL ? NULL : &(node->value);
    }

    if (bst != NULL) {
        *bst = bstDelete(*bst, c->tokens[c->size - 1]);
    }

    printOk();
}

static inline bool validateCheckArguments(Command *c) {
    bool correctArgumentsCount = c->size >= 2 && c->size <= 4;
    char *lastArgument = c->tokens[c->size - 1];
    bool lastArgumentNotAsterisk = (!areStringsEqual(lastArgument, "*"));
    return correctArgumentsCount && lastArgumentNotAsterisk;
}

// declaration first so that the function is visible for mutually-recursive
// functions
static bool recursiveSeekItem(Tree, char **);

static bool bstBfsSeekValue(Tree t, char **tokensLeft) {
    if (t == NULL)
        return false;

    return (bstBfsSeekValue(t->right, tokensLeft) ||
            (recursiveSeekItem(t->value, tokensLeft + 1)) ||
            (bstBfsSeekValue(t->left, tokensLeft)));
}

static bool recursiveSeekItem(Tree bst, char **tokensLeft) {
    if (*tokensLeft == NULL) {
        return true;
    }

    char *token = *tokensLeft;
    if (areStringsEqual(token, "*")) {
        return bstBfsSeekValue(bst, tokensLeft);
    }

    Tree foundNode = bstGet(bst, token);
    if (foundNode == NULL) {
        return false;
    }
    return recursiveSeekItem(foundNode->value, tokensLeft + 1);
}

static void executeCommandCheck(Command *c, Tree bst) {
    if (!validateCheckArguments(c)) {
        printError();
        return;
    }

    char **argsList = safeMalloc(sizeof(char *) * MAX_COMMAND_ARGUMENTS);
    for (unsigned i = 0; i < c->size - 1; i++)
        argsList[i] = c->tokens[i + 1];

    argsList[c->size - 1] = NULL; // iteration guard

    bool resultFound = recursiveSeekItem(bst, argsList);
    free(argsList);

    if (resultFound) {
        printYes();
    } else {
        printNo();
    }
}

/* execute command - strategy design pattern */
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