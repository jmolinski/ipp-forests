#define _GNU_SOURCE

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
            for (unsigned i = 0; i < c->size; i++) {
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

char *getInputLine() {
    char *line = NULL;
    size_t len = 0;
    int nread;

    nread = getline(&line, &len, stdin);
    if (nread == -1) {
        free(line);
        return NULL;
    }

    return line;
}

/* checks if a character is within ASCII 33-255 or is a whitespace */
static inline bool isLegalCharacter(unsigned char c) {
    return (c > 32 || isspace(c));
}

/* checks if the string contains only allowed characters
 * and ends with a newline character */
static bool isInputLineValid(char *s) {
    size_t strLength = strlen(s);
    for (unsigned i = 0; i < strLength; i++) {
        if (!isLegalCharacter(s[i])) {
            return false;
        }
    }

    return s[strLength - 1] == '\n';
}

char *copyString(char *source) {
    size_t strLen = strlen(source);
    char *copy = safeMalloc(sizeof(char) * (strLen + 1));
    strcpy(copy, source);
    return copy;
}

int splitTermsOnDelimiters(char *s, char **terms) {
    char *temp;
    int i = 0;

    while (1) {
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
        if (strcmp(temp, "") != 0) {
            terms[i] = copyString(temp);
            i++;
        }
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

    line = rightStrip(leftStrip(line));

    char **terms = safeMalloc(MAX_COMMAND_TERMS * sizeof(char *));
    int termsCount = splitTermsOnDelimiters(line, terms);

    if (termsCount == -1) {
        // too many terms - raise error
        return createErrorCommand();
    }
    if (termsCount == 0) {
        // empty line; ignore
        free(terms);
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

        if (!isInputLineValid(line)) {
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