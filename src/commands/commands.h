#pragma once
#include "../data_structures/inode.h"
#include <stdlib.h>

#define LS "ls"
#define MKDIR "mkdir"
#define TOUCH "touch"
#define CD "cd"
#define CAT "cat"
#define APPEND "append"
#define EXIT "exit"

void fsLs(size_t id);
void fsMkdir(size_t currentId, char name[FILENAME_LENGTH]);
size_t fsCd(size_t currentId, char name[FILENAME_LENGTH]);
void fsTouch(size_t currentId, char name[FILENAME_LENGTH]);
void fsAppend(int currentDirId, char name[FILENAME_LENGTH], char text[MAX_FILESIZE]);
void fsCat(int currentDirId, char name[FILENAME_LENGTH]);
