#include "sector.h"
#include <stdio.h>
#include <string.h>

size_t getFreeSector() {
    struct Superblock sb = getSuperblock();
    if (sb.freeSectorsNum == 0) {
        printf("Sectors are full");
        return 0;
    }

    size_t nextFreeSector = sb.sectorsNum - sb.freeSectorsNum;

    sb.freeSectorsNum--;
    updateSuperblock(&sb);

    return nextFreeSector;
}

size_t getSectorOffset(size_t n) {
    return SECTORS_OFFSET + n * BLOCK_SIZE;
}

struct NameMapping createNameMapping(size_t id, char* name) {
    struct NameMapping nameMapping;

    strcpy(nameMapping.filename, name);
    nameMapping.inodeId = id;

    return nameMapping;
}

size_t createIndirectBlock() {
    size_t sector = getFreeSector();
    if (sector == 0) {
        return 0;
    }
    struct IndirectBlock indirectBlock;
    size_t indirectSector = getFreeSector();
    indirectBlock.pointers[0] = indirectSector;
    for (int i = 1; i < INDIRECT_BLOCK_SIZE; ++i) {
        indirectBlock.pointers[i] = -1;
    }
    fsWrite(getSectorOffset(sector), &indirectBlock, sizeof(struct IndirectBlock));

    return sector;
}

struct IndirectBlock getIndirectBlock(size_t id) {
    struct IndirectBlock indirectBlock;
    fsRead(getSectorOffset(id), &indirectBlock, sizeof(struct IndirectBlock));
    return indirectBlock;
}
