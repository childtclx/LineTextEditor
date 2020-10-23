#include <stdio.h>
#include <string.h>
#include "editor.h"


int main() {
    Text text = initText();
    Cursor cursor = initCursor();
    String inputString = initString();
    while (true) {
        readString(inputString);
//        printf("command: %s, length: %d\n", inputString->command, strlen(inputString->command));
        if (strlen(inputString->command)) {
            if (!strcmp(inputString->command, "AL")) {
                addLine(text, inputString, cursor);
            } else if (!strcmp(inputString->command, "IL")) {
                insertLine(text, inputString, cursor);
            } else if (!strcmp(inputString->command, "DD")) {
                deleteCurrentLine(text, cursor);
            } else if (!strcmp(inputString->command, "DL")) {
                deletetSetLine(text, inputString, cursor);
            } else if (!strcmp(inputString->command, "IS")) {
                insertString(text, inputString, cursor);
            } else if (!strcmp(inputString->command, "DC")) {
                deleteChar(text, cursor);
            } else if (!strcmp(inputString->command, "MC")) {
                moveCursor(text, cursor, inputString);
            } else if (!strcmp(inputString->command, "PT")) {
                printText(text);
                break;
            } else {
                printf("Command not found!\n");
            }
//            printStringCursor(inputString, cursor);
//            printf("\ntext content:\n");
//            testPrintText(text);
            emptyString(inputString);
//            printf("Empty!\n");
        } else {
            printf("Invalid input!\n");
        }
    }
    return 0;
}