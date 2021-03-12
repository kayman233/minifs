#include "inode.h"

size_t getFreeInodeId() {
    struct Superblock sb = getSuperblock();
    if (sb.freeInodesNum == 0) {
        printf("Inodes are full");
        return 0;
    }

    size_t freeInodeId = sb.inodesNum - sb.freeInodesNum;

    sb.freeInodesNum--;
    updateSuperblock(&sb);

    return freeInodeId;
}

size_t getInodeOffset(size_t id) {
    return INODES_OFFSET + id * INODE_SIZE;
}

struct Inode getInode(size_t id) {
    struct Inode inode;
    fsRead(getInodeOffset(id), (void*) &inode, sizeof(struct Inode));
    return inode;
}

size_t createInodeDir(size_t parentInode, bool isRoot) { // создание Inode для директории
    struct Inode inode;
    inode.id = getFreeInodeId();
    if (inode.id == 0 && !isRoot) { // если заполнено
        return 0;
    }
    inode.type = DIRECTORY;
    inode.fileSize = 0;
    inode.numFiles = isRoot ? 1 : 2;

    inode.directPointers[0] = getFreeSector();
    if (inode.directPointers[0] == 0 && !isRoot) { // если заполнено
        return 0;
    }

    for (int i = 1; i < DIRECT_POINTERS; ++i) {
        inode.directPointers[i] = -1;
    }
    inode.indirectPointer = -1;

    fsWrite(getInodeOffset(inode.id),
            (void*) &inode,
            sizeof(struct Inode));

    struct NameMapping mapping = createNameMapping(inode.id,".");
    int code = writeMapping(inode.id, &mapping);
    if (code == -1 && !isRoot) {
        return 0;
    }
    inode.fileSize += sizeof(struct NameMapping);

    if (!isRoot) {
        mapping = createNameMapping(parentInode,"..");
        code = writeMapping(inode.id, &mapping);
        if (code == -1) {
            return 0;
        }
        inode.fileSize += sizeof(struct NameMapping);
    }

    fsWrite(getInodeOffset(inode.id),
            (void*) &inode,
            sizeof(struct Inode));

    return inode.id;
}

size_t createInodeFile() {
    struct Inode inode;
    inode.id = getFreeInodeId();
    if (inode.id == 0) { // если заполнено
        return 0;
    }

    inode.type = REGULAR_FILE;
    inode.fileSize = 0;
    inode.numFiles = 0;

    inode.directPointers[0] = getFreeSector();
    if (inode.directPointers[0] == 0) { // если заполнено
        return 0;
    }
    for (int i = 1; i < DIRECT_POINTERS; ++i) {
        inode.directPointers[i] = -1;
    }
    inode.indirectPointer = -1;

    fsWrite(getInodeOffset(inode.id),
            (void*) &inode,
            sizeof(struct Inode));

    return inode.id;
}

void addInode(size_t currentInode, size_t newInode, char name[FILENAME_LENGTH]) {
    struct Inode currentDirectory = getInode(currentInode);
    struct NameMapping nameMapping = createNameMapping(newInode, name);

    writeMapping(currentInode, &nameMapping);

    currentDirectory = getInode(currentInode); // обновляем, так как изменился размер директории
    currentDirectory.numFiles++;
    fsWrite(getInodeOffset(currentInode), &currentDirectory, sizeof(struct Inode));
}

int getBlock(size_t id, size_t offset) {
    struct Inode inode = getInode(id);
    size_t numBlock = offset / BLOCK_SIZE;
    if (numBlock < DIRECT_POINTERS) { // если рассматриваем прямые адреса
        if (inode.directPointers[numBlock] == -1) {
            size_t newSector = getFreeSector();
            if (newSector == 0) {
                return -1;
            }
            inode.directPointers[numBlock] = newSector;
            fsWrite(getInodeOffset(id), &inode, sizeof(struct Inode));
        }
        return inode.directPointers[numBlock];
    }

    numBlock -= DIRECT_POINTERS; // теперь счет относительно косвенных адресов

    size_t indirectBlockId;
    if (inode.indirectPointer == -1) {
        indirectBlockId = createIndirectBlock();
        if (indirectBlockId == 0) {
            return -1;
        }
        inode.indirectPointer = indirectBlockId;

        fsWrite(getInodeOffset(id), &inode, sizeof(struct Inode));
    } else {
        indirectBlockId = inode.indirectPointer;
    }

    struct IndirectBlock indirectBlock = getIndirectBlock(indirectBlockId);
    if (indirectBlock.pointers[numBlock] == -1) {
        size_t newSector = getFreeSector();
        if (newSector == 0) {
            return -1;
        }
        indirectBlock.pointers[numBlock] = newSector;

        fsWrite(getSectorOffset(indirectBlockId), &indirectBlock, sizeof(struct IndirectBlock));
    }

    return indirectBlock.pointers[numBlock];
}

void updateFilesize(size_t id, size_t offset, size_t count) {
    struct Inode inode = getInode(id);

    size_t newSize = offset + count;
    if (inode.fileSize < newSize) {
        inode.fileSize = newSize;
    }

    fsWrite(getInodeOffset(id), &inode, sizeof(struct Inode));
}

struct NameMapping getMapping(size_t inodeId, size_t offset) {
    struct NameMapping nameMapping;

    fsReadFile(inodeId, offset, &nameMapping, sizeof(nameMapping));

    return nameMapping;
}

int writeMapping(size_t inodeId, struct NameMapping* nameMapping) {
    struct Inode currentDirectory = getInode(inodeId);
    return fsWriteFile(inodeId, currentDirectory.fileSize, nameMapping, sizeof(struct NameMapping));
}

int fsWriteFile(int id, int offset, void *buf, int count) {
    struct Inode inode = getInode(id);

    if (inode.id != id) {
        printf("File is directory or the file does not exist. Exit with code -1.\n");
        return -1;
    }

    if (offset > inode.fileSize) {
        printf("Wrong offset\n");
        return -1;
    }

    while(count > 0) {
        size_t blockToWrite = getBlock(id, offset);
        if (blockToWrite == -1) {
            return -1;
        }
        int occupiedSpace = offset % BLOCK_SIZE;
        int availableSpaceToWrite = BLOCK_SIZE - occupiedSpace;
        int bytesToWrite = availableSpaceToWrite < count ? availableSpaceToWrite : count;
        fsWrite(getSectorOffset(blockToWrite) + occupiedSpace, buf, bytesToWrite);

        updateFilesize(id, offset, bytesToWrite);

        offset += bytesToWrite;
        buf += bytesToWrite;
        count -= bytesToWrite;
    }

    return 0;
}

int fsReadFile(int id, int offset, void *buf, int count) {
    struct Inode inode = getInode(id);

    if (inode.id != id) {
        return -1;
    }

    if (offset + count > inode.fileSize) {
        return -1;
    }

    size_t readCount = 0;

    while(count > 0) {
        size_t blockToRead = getBlock(id, offset);
        if (blockToRead == -1) {
            return -1;
        }
        int occupiedSpace = offset % BLOCK_SIZE;
        int availableSpaceToRead = BLOCK_SIZE - occupiedSpace;
        int bytesToRead = availableSpaceToRead < count ? availableSpaceToRead : count;
        fsRead(getSectorOffset(blockToRead) + occupiedSpace, buf + readCount, bytesToRead);

        offset += bytesToRead;
        readCount += bytesToRead;
        count -= bytesToRead;
    }

    return 0;
}
