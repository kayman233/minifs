#include "fsio.h"

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

void fsRead(size_t offset, void* data, size_t size) {
    int fd = open(FS_NAME, O_RDWR, 660);
    if (fd == -1) {
        printf("Abort");
        exit(0);
    }

    lseek(fd, offset, SEEK_SET);
    read(fd, data, size);
    close(fd);
}

void fsWrite(size_t offset, void* data, size_t size) {
    int fd = open(FS_NAME, O_RDWR, 660);
    if (fd == -1) {
        printf("Abort");
        exit(0);
    }

    lseek(fd, offset, SEEK_SET);
    write(fd, data, size);
    close(fd);
}