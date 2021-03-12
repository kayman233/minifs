#include "commands.h"
#include <string.h>
#include <stdlib.h>

int fsOpen(size_t id, char* path) {
    if (path == NULL) {
        return -1;
    }

    char* token = strsep(&path, "/");
    size_t mappingSize = sizeof(struct NameMapping);

    while (token != NULL) {
        if (strcmp(token, "") == 0) {
            token = strsep(&path, "/");
            continue;
        }

        struct Inode currentDirectory = getInode(id);
        size_t i;
        for (i = 0; i < currentDirectory.numFiles; ++i) {
            struct NameMapping mapping = getMapping(id, i * mappingSize);

            if (strcmp(token, mapping.filename) == 0) {
                id = mapping.inodeId;
                break;
            }
        }

        if (i == currentDirectory.numFiles) { // не нашли
            return -1;
        }

        token = strsep(&path, "/");
    }

    return id;
}

void fsLs(size_t id) {
    struct Inode currentDirectory = getInode(id);
    if (currentDirectory.id != id) { // проверка того, что данный inode есть
        printf("Wrong directory");
        return;
    }
    for (int i = 0; i < currentDirectory.numFiles; ++i) {
        size_t mappingSize = sizeof(struct NameMapping);
        struct NameMapping mapping = getMapping(id, i * mappingSize);
        printf("%s ", mapping.filename);
    }
}

void fsMkdir(size_t currentId, char name[FILENAME_LENGTH]) {
    struct Inode currentDirectory = getInode(currentId);
    size_t mappingSize = sizeof(struct NameMapping);
    for (int i = 0; i < currentDirectory.numFiles; i++) {
        struct NameMapping mapping = getMapping(currentId, i * mappingSize);
        if(strcmp(name, mapping.filename)==0) {
            printf("This directory already exists");
            return;
        }
    }

    if (currentDirectory.fileSize + sizeof(struct NameMapping) > MAX_FILESIZE) {
        printf("Directory is full");
        return;
    }

    size_t newInodeId = createInodeDir(currentId, false);
    if (newInodeId == 0) {
        return;
    }
    addInode(currentId, newInodeId, name);
}

size_t fsCd(size_t currentId, char name[FILENAME_LENGTH]) {
    if (strcmp(name, "/") == 0) {
        return 0;
    }

    if (name[0] == '/') {
        currentId = 0;
    }

    int foundId = fsOpen(currentId, name);
    if (foundId == -1) {
        printf("Wrong path");
        return currentId;
    } else {
        if (getInode(foundId).type == REGULAR_FILE) {
            printf("This is not a directory.");
            return currentId;
        }

        return foundId;
    }
}

void fsTouch(size_t currentId, char name[FILENAME_LENGTH]) {
    struct Inode currentDirectory = getInode(currentId);
    size_t mappingSize = sizeof(struct NameMapping);
    for (int i = 0; i < currentDirectory.numFiles; i++) {
        struct NameMapping mapping = getMapping(currentId,i * mappingSize);
        if(strcmp(name, mapping.filename)==0) {
            printf("This file already exists");
            return;
        }
    }

    if (currentDirectory.fileSize + sizeof(struct NameMapping) > MAX_FILESIZE) {
        printf("Directory is full");
        return;
    }

    size_t newInodeId = createInodeFile();
    addInode(currentId, newInodeId, name);
}

void fsAppend(int currentDirId, char name[FILENAME_LENGTH], char text[MAX_FILESIZE]) {
    int num = fsOpen(currentDirId, name);
    struct Inode inode = getInode(num);
    if (num == -1) {
        printf("File not found. ");
        return;
    }

    if (inode.fileSize == MAX_FILESIZE) {
        printf("File is full");
        return;
    }

    int length = (int) strlen(text);
    if (inode.fileSize + strlen(text) > MAX_FILESIZE) {
        length = MAX_FILESIZE - inode.fileSize;
    }

    int code = fsWriteFile(num, inode.fileSize, text, length);
    if (code == -1) {
        printf("Error\n");
    }
}

void fsCat(int currentDirId, char name[FILENAME_LENGTH]) {
    int id = fsOpen(currentDirId, name);
    if (id == -1) {
        printf("File not found. ");
        return;
    }
    struct Inode inode = getInode(id);
    char *buf = malloc(inode.fileSize);
    int code = fsReadFile(inode.id, 0, buf, inode.fileSize);

    if (code == -1) {
        printf("Error");
        return;
    } else {
        printf("%s", buf);
        return;
    }
}
