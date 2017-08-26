#include <libk/heap.h>
#include <test/unit.h>

inline static bool map_check(unsigned char* bitmap, size_t block) {
  return bitmap[block / 8] & (1 << (block % 8));
}

NEW_SUITE(HeapTest, 7);

SETUP_SUITE() {
  // Force the tests to start with an empty Heap Page. This way,
  // we can actually test the bitmap is built correctly independent
  // of what was running beforehand
  force_empty_heap_page();
}
// extern free_list_t free_list_;

TEST(EmptyMalloc) { 
  EXPECT_EQ(NULL, kmalloc(0));
}

TEST(MallocLargerThanBlockLimit) {
  EXPECT_EQ(NULL, kmalloc(HEAP_BLOCK_COUNT * HEAP_BLOCK_SIZE + 1));
}

TEST(MallocFreeExactBlockSize) {
  size_t size = sizeof(int) * HEAP_BLOCK_SIZE;
  int* ptr = kmalloc(size);

  size_t expected_allocated_block_count = size / HEAP_BLOCK_SIZE;
  heap_page_t* heap_page = get_heap_block_metadata(ptr);
  EXPECT_EQ(heap_page->num_available_blocks, 
      HEAP_BLOCK_COUNT - expected_allocated_block_count);
  
  // Expect the alloced bitmap has the alloced blocks bits set
  for (size_t i = 0; i < expected_allocated_block_count; i++) {
    EXPECT_TRUE(map_check(heap_page->alloced_block_bitmap, i));
  }

  // Expect the first_alloced_bitmap has only the first block bit set
  EXPECT_TRUE(map_check(heap_page->first_alloced_bitmap, 0));
  for (size_t i = 1; i < expected_allocated_block_count; i++) {
    EXPECT_FALSE(map_check(heap_page->first_alloced_bitmap, i));
  }

  // Clean-up
  kfree(ptr);
  EXPECT_EQ(heap_page->num_available_blocks, HEAP_BLOCK_COUNT);
  // Assert we cleaned the bitmaps
  for (size_t i = 0; i < expected_allocated_block_count; i++) {
    EXPECT_FALSE(map_check(heap_page->alloced_block_bitmap, i));
    EXPECT_FALSE(map_check(heap_page->first_alloced_bitmap, i));
  }
}

TEST(MallocFreeNonExactBlockSize) {
  size_t size = sizeof(int) * (HEAP_BLOCK_SIZE + 5);
  int* ptr = kmalloc(size);

  size_t expected_allocated_block_count = size / HEAP_BLOCK_SIZE + 1;
  heap_page_t* heap_page = get_heap_block_metadata(ptr);
  EXPECT_EQ(heap_page->num_available_blocks, 
      HEAP_BLOCK_COUNT - expected_allocated_block_count);

  // Expect the alloced bitmap has the alloced blocks bits set
  for (size_t i = 0; i < expected_allocated_block_count; i++) {
    EXPECT_TRUE(map_check(heap_page->alloced_block_bitmap, i));
  }

  // Expect the first_alloced_bitmap has only the first block bit set
  EXPECT_TRUE(map_check(heap_page->first_alloced_bitmap, 0));
  for (size_t i = 1; i < expected_allocated_block_count; i++) {
    EXPECT_FALSE(map_check(heap_page->first_alloced_bitmap, i));
  }

  // Clean-up
  kfree(ptr);
  EXPECT_EQ(heap_page->num_available_blocks, HEAP_BLOCK_COUNT);
  // Assert we cleaned the bitmaps
  for (size_t i = 0; i < expected_allocated_block_count; i++) {
    EXPECT_FALSE(map_check(heap_page->alloced_block_bitmap, i));
    EXPECT_FALSE(map_check(heap_page->first_alloced_bitmap, i));
  }
}

TEST(MultipleMallocAndFrees) {
  size_t alloc1_size = sizeof(int) * 32; // 8 blocks
  size_t alloc2_size = sizeof(int) * 33; // 9 blocks
  size_t alloc3_size = sizeof(int) * 10; // 3 block

  int* ptr1 = kmalloc(alloc1_size);
  int* ptr2 = kmalloc(alloc2_size);
  int* ptr3 = kmalloc(alloc3_size);

  size_t expected_allocated_block_count = 8 + 9 + 3;
  heap_page_t* heap_page = get_heap_block_metadata(ptr1);
  EXPECT_EQ(heap_page->num_available_blocks, 
      HEAP_BLOCK_COUNT - expected_allocated_block_count);

  // Expect the alloced bitmap has the alloced blocks bits set
  for (size_t i = 0; i < expected_allocated_block_count; i++) {
    EXPECT_TRUE(map_check(heap_page->alloced_block_bitmap, i));
  }

  // Expect the first_alloced_bitmap has only the first block bit set
  // for each alloc
  for (size_t i = 0; i < expected_allocated_block_count; i++) {
    if (i == 0 || i == 8 || i == 17) {
      EXPECT_TRUE(map_check(heap_page->first_alloced_bitmap, i));
    } else {
      EXPECT_FALSE(map_check(heap_page->first_alloced_bitmap, i));
    }
  }

  kfree(ptr1);
  kfree(ptr2);
  kfree(ptr3);
  // Assert we cleaned the bitmaps
  for (size_t i = 0; i < expected_allocated_block_count; i++) {
    EXPECT_FALSE(map_check(heap_page->alloced_block_bitmap, i));
    EXPECT_FALSE(map_check(heap_page->first_alloced_bitmap, i));
  }
}

TEST(MallocMaxBlockSize) {
  size_t size = HEAP_BLOCK_COUNT * HEAP_BLOCK_SIZE;
  int* ptr = kmalloc(size);

  size_t expected_allocated_block_count = size / HEAP_BLOCK_SIZE;
  heap_page_t* heap_page = get_heap_block_metadata(ptr);
  EXPECT_EQ(heap_page->num_available_blocks, 
      HEAP_BLOCK_COUNT - expected_allocated_block_count);

   // Expect the alloced bitmap has the alloced blocks bits set
  for (size_t i = 0; i < expected_allocated_block_count; i++) {
    EXPECT_TRUE(map_check(heap_page->alloced_block_bitmap, i));
  }

  // Expect the first_alloced_bitmap has only the first block bit set
  EXPECT_TRUE(map_check(heap_page->first_alloced_bitmap, 0));
  for (size_t i = 1; i < expected_allocated_block_count; i++) {
    EXPECT_FALSE(map_check(heap_page->first_alloced_bitmap, i));
  }

  // Clean-up
  kfree(ptr);
  EXPECT_EQ(heap_page->num_available_blocks, HEAP_BLOCK_COUNT);
  // Assert we cleaned the bitmaps
  for (size_t i = 0; i < expected_allocated_block_count; i++) {
    EXPECT_FALSE(map_check(heap_page->alloced_block_bitmap, i));
    EXPECT_FALSE(map_check(heap_page->first_alloced_bitmap, i));
  }
}

TEST(InterleavingMallocFreeAndMalloc) {
  // Alloc five allocations of size of increasing block sizes
  int* ptr1 = kmalloc(HEAP_BLOCK_SIZE * 1);
  int* ptr2 = kmalloc(HEAP_BLOCK_SIZE * 2);
  int* ptr3 = kmalloc(HEAP_BLOCK_SIZE * 3);
  int* ptr4 = kmalloc(HEAP_BLOCK_SIZE * 4);
  int* ptr5 = kmalloc(HEAP_BLOCK_SIZE * 5);

  heap_page_t* heap_page = get_heap_block_metadata(ptr1);
  EXPECT_EQ(heap_page->num_available_blocks, HEAP_BLOCK_COUNT - 15);

  // Free ptr4
  kfree(ptr4);

  // Alloc a new ptr of size equal to 3 blocks
  int* ptr6 = kmalloc(HEAP_BLOCK_SIZE * 3);

  // Expect ptr6 to be allocated in between ptr 3 and 5
  EXPECT_GT(ptr6, ptr3);
  EXPECT_LT(ptr6, ptr5);

  // Alloc a new ptr of size of 1 blocks
  int* ptr7 = kmalloc(HEAP_BLOCK_SIZE * 1);

  // Expect ptr6 to be allocated in between ptr 6 and 5
  EXPECT_GT(ptr7, ptr6);
  EXPECT_LT(ptr7, ptr5);

  // Finally, allocated a new ptr of size 6
  int* ptr8 = kmalloc(HEAP_BLOCK_SIZE * 6);

  // Expect ptr8 to be allocated in between after ptr 5
  EXPECT_GT(ptr8, ptr5);

  // Free everything and assert the resulting bitmap is empty
  kfree(ptr1);
  kfree(ptr2);
  kfree(ptr3);
  kfree(ptr5);
  kfree(ptr6);
  kfree(ptr7);
  kfree(ptr8);
  EXPECT_EQ(heap_page->num_available_blocks, HEAP_BLOCK_COUNT);

  // Assert we cleaned the bitmaps
  for (size_t i = 0; i < HEAP_BLOCK_COUNT; i++) {
    EXPECT_FALSE(map_check(heap_page->alloced_block_bitmap, i));
    EXPECT_FALSE(map_check(heap_page->first_alloced_bitmap, i));
  }
}

TEST(FreeNull) {
  kfree(NULL);
}

TEST(FreeNotMallocedDoesntWork) {
  size_t size = HEAP_BLOCK_COUNT * HEAP_BLOCK_SIZE;
  int* ptr = kmalloc(size);

  size_t expected_allocated_block_count = size / HEAP_BLOCK_SIZE;
  heap_page_t* heap_page = get_heap_block_metadata(ptr);
  EXPECT_EQ(heap_page->num_available_blocks, 
      HEAP_BLOCK_COUNT - expected_allocated_block_count);

  // Tries to free a pointer inside an allocated block, but that isn't the
  // block start. In the future it should abort, for now it just does nothing
  kfree(ptr + 1);

  // Assert we didn't actually free anything
  EXPECT_EQ(heap_page->num_available_blocks, 
      HEAP_BLOCK_COUNT - expected_allocated_block_count);

  kfree(ptr);
}


END_SUITE();

void test_heap() { RUN_SUITE(HeapTest); }