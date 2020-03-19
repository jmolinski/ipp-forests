#ifndef FOREST_COMMANDS_H
#define FOREST_COMMANDS_H

#include "bst.h"
#include "input_interface.h"

/* execute s given command on a tree
 * command must not be NULL */
void executeCommandStrategy(Command *, Tree *);

#endif // FOREST_COMMANDS_H
