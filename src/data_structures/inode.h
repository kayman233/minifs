#pragma once
#include "sector.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

enum file_type {
    REGULAR_FILE,
    DIRECTORY
};

struct Inode {
    size_t id;
    enum file_type type;
    size_t fileSize;
    int directPointers[DIRECT_POINTERS];
    int indirectPointer;
    size_t numFiles;
};

size_t getFreeInodeId();
size_t getInodeOffset(size_t id);

struct Inode getInode(size_t id);

size_t createInodeDir(size_t parentInode, bool isRoot);
size_t createInodeFile();

int getBlock(size_t id, size_t offset);

void addInode(size_t currentInode, size_t newInode, char name[FILENAME_LENGTH]); // добавление в директории нового файла
void updateFilesize(size_t id, size_t offset, size_t count);

struct NameMapping getMapping(size_t inodeId, size_t offset);
int writeMapping(size_t inodeId, struct NameMapping* nameMapping);

int fsWriteFile(int id, int offset, void *buf, int count);
int fsReadFile(int id, int offset, void *buf, int count);
