#include <libk/heap.h>
#include <test/unit.h>

NEW_SUITE(HeapTest, 7);

extern free_list_t free_list_;

TEST(EmptyMalloc) { EXPECT_EQ(NULL, kmalloc(0)); }

TEST(Malloc) {
  size_t size = sizeof(int) * 10;
  int* ptr = kmalloc(size);

  meta_alloc_t* metadata = get_metadata((virtual_addr)ptr);
  EXPECT_EQ(metadata->size, size);
  EXPECT_EQ(metadata->checksum, MALLOCED_CHECKSUM);
  EXPECT_EQ(metadata->next, NULL);

  // Clean-up
  kfree(ptr);
}

TEST(FreeNull) { kfree(NULL); }

TEST(FreeNotMallocedDoesntWork) {
  // TODO(psamora) When we add abort,  figure out how to test this
  char* ptr = kmalloc(1);
  meta_alloc_t* metadata = get_metadata((virtual_addr)ptr);

  // Since we are not freeing the right pointer, nothing will happen
  kfree(ptr + 1);

  EXPECT_GTE(metadata->size, 1);
  EXPECT_EQ(metadata->checksum, MALLOCED_CHECKSUM);
  EXPECT_EQ(metadata->next, NULL);

  // Clean-up
  kfree(ptr);
}

TEST(FreeMalloced) {
  char* ptr = kmalloc(25);
  meta_alloc_t* metadata = get_metadata((virtual_addr)ptr);
  kfree(ptr);
  EXPECT_GTE(metadata->size, 25);
  EXPECT_NE(metadata->checksum, MALLOCED_CHECKSUM);
  EXPECT_NE(metadata->next, NULL);
}

TEST(FreeListCorrectlyPopulated) {
  // Populate the free_list with 10 free blocks of increasing size
  for (size_t i = 1; i <= 10; i++) {
    char* ptr = kmalloc(i);
    kfree(ptr);
  }

  meta_alloc_t* cur_metadata = free_list_.head;

  // Check the first 10 free blocks in the FreeList are in decreasing order
  for (size_t i = 10; i > 9; i--) {
    EXPECT_GTE(cur_metadata->size, i);
    EXPECT_NE(cur_metadata->checksum, MALLOCED_CHECKSUM);
    EXPECT_NE(cur_metadata->next, NULL);
    cur_metadata = cur_metadata->next;
  }
}

TEST(SplittingBlockOfMemory) {
  size_t large_block_size = 200;

  // Allocate large block and free it so it is in the front of the FreeList
  char* ptr = kmalloc(large_block_size);
  kfree(ptr);

  // Allocate 10 blocks, asserting the block in the front keeps decreasing
  // since we are just splitting the block above
  for (size_t i = 1; i <= 10; i++) {
    char* ptr = kmalloc(i);

    large_block_size -= i + META_ALLOC_SIZE;

    meta_alloc_t* free_metadata = free_list_.head;
    EXPECT_GTE(free_metadata->size, large_block_size);
    EXPECT_NE(free_metadata->checksum, MALLOCED_CHECKSUM);

    // Clean-up
    kfree(ptr);
  }
}

END_SUITE();

void test_heap() { RUN_SUITE(HeapTest); }