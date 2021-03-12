#pragma once
#include "superblock.h"

// структуры, которые будут лежать в секторах
struct NameMapping {
    size_t inodeId;
    char filename[FILENAME_LENGTH];
};

struct IndirectBlock {
    int pointers[INDIRECT_BLOCK_SIZE];
};

size_t getFreeSector();
size_t getSectorOffset(size_t n);

struct NameMapping createNameMapping(size_t id, char* name);

size_t createIndirectBlock();
struct IndirectBlock getIndirectBlock(size_t id);
