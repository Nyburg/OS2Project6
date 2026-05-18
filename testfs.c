#include "block.h"
#include "ctest.h"
#include "image.h"

#include <string.h>
#include <unistd.h>

#define TEST_IMAGE "testfs.img"

static int test_image_open_close(void)
{
    int fd = image_open(TEST_IMAGE, 1);

    CTEST_ASSERT(fd >= 0);
    CTEST_ASSERT(image_close() == 0);

    unlink(TEST_IMAGE);

    return 0;
}

static int test_block_write_read(void)
{
    unsigned char write_block[BLOCK_SIZE];
    unsigned char read_block[BLOCK_SIZE];
    int i;

    CTEST_ASSERT(image_open(TEST_IMAGE, 1) >= 0);

    for (i = 0; i < BLOCK_SIZE; i++) {
        write_block[i] = (unsigned char)(i % 256);
        read_block[i] = 0;
    }

    bwrite(0, write_block);
    bread(0, read_block);

    CTEST_ASSERT(memcmp(write_block, read_block, BLOCK_SIZE) == 0);
    CTEST_ASSERT(image_close() == 0);

    unlink(TEST_IMAGE);

    return 0;
}

static int test_multiple_blocks(void)
{
    unsigned char block_one[BLOCK_SIZE];
    unsigned char block_two[BLOCK_SIZE];
    unsigned char read_block[BLOCK_SIZE];
    int i;

    CTEST_ASSERT(image_open(TEST_IMAGE, 1) >= 0);

    for (i = 0; i < BLOCK_SIZE; i++) {
        block_one[i] = 'A';
        block_two[i] = 'B';
        read_block[i] = 0;
    }

    bwrite(1, block_one);
    bwrite(2, block_two);

    bread(1, read_block);
    CTEST_ASSERT(memcmp(block_one, read_block, BLOCK_SIZE) == 0);

    bread(2, read_block);
    CTEST_ASSERT(memcmp(block_two, read_block, BLOCK_SIZE) == 0);

    CTEST_ASSERT(image_close() == 0);

    unlink(TEST_IMAGE);

    return 0;
}

int main(void)
{
    int failures = 0;

    CTEST_RUN(test_image_open_close);
    CTEST_RUN(test_block_write_read);
    CTEST_RUN(test_multiple_blocks);

    if (failures == 0) {
        printf("All tests passed\n");
    }

    return failures;
}