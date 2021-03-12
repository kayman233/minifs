#pragma once

#include "../params.h"
#include "../io/fsio.h"
#include <stdlib.h>
#include <stdio.h>

struct Superblock {
    int magicNumber;
    size_t blockSize;
    size_t inodeSize;
    size_t sectorsNum;
    size_t inodesNum;
    size_t freeSectorsNum;
    size_t freeInodesNum;
};

struct Superblock getSuperblock();
void updateSuperblock(struct Superblock* sb);
