#include "output_interface.h"
#include <stdio.h>

void printError() {
    fprintf(stderr, "ERROR\n");
}

void printOk() {
    printLine("OK");
}

void printNo() {
    printLine("NO");
}

void printYes() {
    printLine("YES");
}

void printLine(char *s) {
    printf("%s\n", s);
}