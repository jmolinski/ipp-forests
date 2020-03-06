#include "input_handler.h"
#include "safe_malloc.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WHITE_DELIMITERS "\t\v\f\r \n"
#define MAX_COMMAND_TERMS 5

void freeCommand(Command *c) {
    if (c != NULL) {
        if (!c->error) {
            for (int i = 0; i < c->size; i++) {
                free(c->tokens[i]);
            }
            free(c->tokens);
        }
        free(c);
    }
}

char *leftStrip(char *s) {
    if (s == NULL) {
        return s;
    }

    size_t charsToTrim;
    if ((charsToTrim = strspn(s, WHITE_DELIMITERS)) > 0) {
        size_t len = strlen(s);
        if (charsToTrim == len) {
            s[0] = '\0';
        } else {
            memmove(s, s + charsToTrim, len + 1 - charsToTrim);
        }
    }

    return s;
}

char *rightStrip(char *s) {
    if (s == NULL) {
        return s;
    }

    int i = strlen(s) - 1;
    while (i >= 0 && strchr(WHITE_DELIMITERS, s[i]) != NULL) {
        s[i] = '\0';
        i--;
    }
    return s;
}

char *strip(char *s) {
    return rightStrip(leftStrip(s));
}

char *getInputLine() {
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;

    nread = getline(&line, &len, stdin);
    if (nread == -1) {
        free(line);
        return NULL;
    }

    return line;
}

static bool isLegalCharacter(unsigned char c) {
    return (c > 32 || isspace(c));
}

static bool areAllCharactersLegal(char *s) {
    size_t strLength = strlen(s);
    for (int i = 0; i < strLength; i++) {
        if (!isLegalCharacter(s[i])) {
            return false;
        }
    }

    return true;
}

char *copyString(char *source) {
    size_t strLen = strlen(source);
    char *copy = safeMalloc(sizeof(char) * (strLen + 1));
    strcpy(copy, source);
    return copy;
}

int splitTermsOnDelimiters(char *s, char **terms) {
    char *temp;
    int i;

    for (i = 0;; i++) {
        if (i >= MAX_COMMAND_TERMS) {
            for (int j = 0; j < MAX_COMMAND_TERMS; j++)
                free(terms[j]);
            free(terms);
            return -1;
        }

        temp = strsep(&s, WHITE_DELIMITERS);
        if (temp == NULL) {
            break;
        }
        terms[i] = copyString(temp);
    }

    return i;
}

Command *createCommand(char **terms, size_t size, bool error) {
    Command *command = safeMalloc(sizeof(Command));
    command->tokens = terms;
    command->size = size;
    command->error = error;
    return command;
}

static inline Command *createErrorCommand() {
    return createCommand(NULL, 0, true);
}

Command *tokenizeLine(char *line) {
    if (line == NULL || line[0] == '\0' || line[0] == '#') {
        // empty line or starts with #; should be ignored
        return NULL;
    }

    line = strip(line);

    char **terms = safeMalloc(MAX_COMMAND_TERMS * sizeof(char *));
    int termsCount = splitTermsOnDelimiters(line, terms);

    if (termsCount == -1) {
        // too many terms - raise error
        return createErrorCommand();
    }
    if (termsCount == 0) {
        // empty line; ignore
        return NULL;
    }

    return createCommand(terms, termsCount, false);
}

Command *getNextCommand() {
    char *line = NULL;
    Command *command = NULL;

    while (true) {
        line = getInputLine();
        if (line == NULL) {
            return NULL;
        }

        if (!areAllCharactersLegal(line)) {
            return createErrorCommand();
        }

        command = tokenizeLine(line);
        free(line);

        if (command != NULL) {
            // command will be NULL if the line was ignored or empty
            return command;
        }
    }
}