#include "block.h"
#include "image.h"

#include <stdlib.h>
#include <unistd.h>

unsigned char *bread(int block_num, unsigned char *block)
{
    off_t offset = (off_t)block_num * BLOCK_SIZE;
    ssize_t bytes_read;

    if (lseek(image_fd, offset, SEEK_SET) == -1) {
        exit(1);
    }

    bytes_read = read(image_fd, block, BLOCK_SIZE);

    if (bytes_read == -1) {
        exit(1);
    }

    return block;
}

void bwrite(int block_num, unsigned char *block)
{
    off_t offset = (off_t)block_num * BLOCK_SIZE;
    ssize_t bytes_written;

    if (lseek(image_fd, offset, SEEK_SET) == -1) {
        exit(1);
    }

    bytes_written = write(image_fd, block, BLOCK_SIZE);

    if (bytes_written != BLOCK_SIZE) {
        exit(1);
    }
}