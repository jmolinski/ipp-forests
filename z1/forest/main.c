/* author: Jakub Molinski 419502
 * description: https://moodle.mimuw.edu.pl/mod/assign/view.php?id=21480
*/

#include "commands.h"
#include "input_interface.h"
#include <string.h>

int main() {
    Command *command;
    Tree forests = NULL;

    while ((command = getNextCommand()) != NULL) {
        executeCommandStrategy(command, &forests);
        freeCommand(command);
    }

    freeTree(forests);

    return 0;
}
