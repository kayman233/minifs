#include "superblock.h"

struct Superblock getSuperblock() {
    struct Superblock sb;
    fsRead(SUPERBLOCK_OFFSET,(void*) &sb, sizeof(struct Superblock));
    return sb;
}

void updateSuperblock(struct Superblock* sb) {
    fsWrite(SUPERBLOCK_OFFSET, sb, sizeof(struct Superblock));
}
