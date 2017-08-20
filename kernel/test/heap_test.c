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

TEST(EmptyMalloc) { EXPECT_EQ(NULL, kmalloc(0)); }

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
}

// TEST(FreeNull) { kfree(NULL); }

// TEST(FreeNotMallocedDoesntWork) {
//   // TODO(psamora) When we add abort,  figure out how to test this
//   char* ptr = kmalloc(1);
//   meta_alloc_t* metadata = get_metadata((virtual_addr)ptr);

//   // Since we are not freeing the right pointer, nothing will happen
//   kfree(ptr + 1);

//   EXPECT_GTE(metadata->size, 1);
//   EXPECT_EQ(metadata->checksum, MALLOCED_CHECKSUM);
//   EXPECT_EQ(metadata->next, NULL);

//   // Clean-up
//   kfree(ptr);
// }

// TEST(FreeMalloced) {
//   char* ptr = kmalloc(25);
//   meta_alloc_t* metadata = get_metadata((virtual_addr)ptr);
//   kfree(ptr);
//   EXPECT_GTE(metadata->size, 25);
//   EXPECT_NE(metadata->checksum, MALLOCED_CHECKSUM);
//   EXPECT_NE(metadata->next, NULL);
// }

// TEST(FreeListCorrectlyPopulated) {
//   // Populate the free_list with 10 free blocks of increasing size
//   for (size_t i = 1; i <= 10; i++) {
//     char* ptr = kmalloc(i);
//     kfree(ptr);
//   }

//   meta_alloc_t* cur_metadata = free_list_.head;

//   // Check the first 10 free blocks in the FreeList are in decreasing order
//   for (size_t i = 10; i > 9; i--) {
//     EXPECT_GTE(cur_metadata->size, i);
//     EXPECT_NE(cur_metadata->checksum, MALLOCED_CHECKSUM);
//     EXPECT_NE(cur_metadata->next, NULL);
//     cur_metadata = cur_metadata->next;
//   }
// }

// TEST(SplittingBlockOfMemory) {
//   size_t large_block_size = 200;

//   // Allocate large block and free it so it is in the front of the FreeList
//   char* ptr = kmalloc(large_block_size);
//   kfree(ptr);

//   // Allocate 10 blocks, asserting the block in the front keeps decreasing
//   // since we are just splitting the block above
//   for (size_t i = 1; i <= 10; i++) {
//     char* ptr = kmalloc(i);

//     large_block_size -= i + META_ALLOC_SIZE;

//     meta_alloc_t* free_metadata = free_list_.head;
//     EXPECT_GTE(free_metadata->size, large_block_size);
//     EXPECT_NE(free_metadata->checksum, MALLOCED_CHECKSUM);

//     // Clean-up
//     kfree(ptr);
//   }
// }

END_SUITE();

void test_heap() { RUN_SUITE(HeapTest); }