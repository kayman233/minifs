#include "./src/params.h"
#include "./src/commands/commands.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h> 

int main() {
    size_t currentDirectory = 0;
    size_t maxSize = FILENAME_MAX;
    char* forbidenFileName = "/\\:*?<>|+=[];, ";
    char* forbidenDirectoryName = "/\\:*?<>|+=[];,. ";

    while (true) {
        char command[10];
        char* str = NULL;
        printf("\n$ ");
        int result = getline(&str, &maxSize, stdin);

        if (result == -1) {
            printf("Input read error");
            return -1;
        } else if (str[0] == '\n') {
            command[0] = '\0';
        } else {
            str[result-1] = '\0';
            char* token = strtok(str, " ");
            strcpy(command, token);
        }

        if (strcmp(command, LS) == 0) {
            fsLs(currentDirectory);
            continue;
        }

        if (strcmp(command, MKDIR) == 0) {
            char name[FILENAME_LENGTH];
            char* token = strtok(NULL, "");

            if (token == NULL) {
                printf("Please enter a valid name");
                continue;
            }

            if (strpbrk(token, forbidenDirectoryName) != NULL) {
                printf("Please enter a valid name");
                continue;
            }

            if (strlen(token) > FILENAME_LENGTH) {
                printf("Please enter a valid name - this name is too long");
                continue;
            }

            strcpy(name, token);

            fsMkdir(currentDirectory, name);
            continue;
        }

        if (strcmp(command, CD) == 0) {
            char name[FILENAME_LENGTH];
            char* token = strtok(NULL, "");

            if (token == NULL) {
                printf("Please enter a valid name");
                continue;
            }

            strcpy(name, token);

            currentDirectory = fsCd(currentDirectory, name);
            continue;
        }

        if (strcmp(command, TOUCH) == 0) {
            char name[FILENAME_LENGTH];
            char* token = strtok(NULL, "");

            if (token == NULL) {
                printf("Please enter a valid name");
                continue;
            }

            if (strpbrk(token, forbidenFileName) != NULL) {
                printf("Please enter a valid name");
                continue;
            }

            if (strlen(token) > FILENAME_LENGTH) {
                printf("Please enter a valid name - this name is too long");
                continue;
            }

            strcpy(name, token);

            fsTouch(currentDirectory, name);
            continue;
        }

        if (strcmp(command, EXIT) == 0) {
            break;
        }

        if(strcmp(command, APPEND) == 0) {
            char* token = strtok(NULL, " ");
            char name[FILENAME_LENGTH];

            strcpy(name, token);
            if (token == NULL) {
                printf("Wrong file");
                continue;
            }

            char text[MAX_FILESIZE];
            token = strtok(NULL, "");
            if (token==NULL) {
                printf("Wrong text");
                continue;
            }
            strcpy(text, token);

            fsAppend(currentDirectory, name, text);
            continue;
        }

        if(strcmp(command, CAT) == 0) {
            char name[FILENAME_LENGTH];
            char* token = strtok(NULL, "");

            strcpy(name, token);
            if (token == NULL) {
                printf("Specify name of the file.");
                continue;
            }

            fsCat(currentDirectory, name);
            continue;
        }

        printf("Unknown command");
    }
    return 0;
}