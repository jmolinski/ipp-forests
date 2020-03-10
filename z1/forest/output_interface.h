#ifndef FOREST_OUTPUT_INTERFACE_H
#define FOREST_OUTPUT_INTERFACE_H

/* prints 'error' message to stderr */
void printError();

/* prints 'ok' message to stdout */
void printOk();

/* prints 'no' message to stdout */
void printNo();

/* prints 'yes' message to stdout */
void printYes();

/* prints a string and a newline stdout */
void printLine(char *);

#endif // FOREST_OUTPUT_INTERFACE_H
