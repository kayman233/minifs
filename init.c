#include "./src/params.h"
#include "./src/data_structures/superblock.h"
#include "./src/data_structures/inode.h"
#include <stdio.h>

void init_superblock() {
    struct Superblock sb;
    sb.magicNumber = MAGIC_NUMBER;
    sb.blockSize = BLOCK_SIZE;
    sb.inodeSize = INODE_SIZE;
    sb.sectorsNum = N_SECTORS;
    sb.inodesNum = N_INODES;
    sb.freeSectorsNum = N_SECTORS;
    sb.freeInodesNum = N_INODES;
    updateSuperblock(&sb);
}

void init_root_dir() {
    createInodeDir(0, true);
}

int main(int argc, char* argv[]) {
    FILE *fp;
    fp  = fopen ("./minifs", "w+");
    fclose(fp);

    init_superblock();
    init_root_dir();

    return 0;
}
