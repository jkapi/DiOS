#include <test/unit.h>
#include <libk/phys_mem.h>

SUITE(PhysMemTest);

TEST(AllocBlock) {
  physical_addr addr = alloc_block();
  EXPECT_TRUE("Basic allocation test/1", addr);
  EXPECT_TRUE("Basic allocation test/2", is_alloced(addr));
  return 0;
}

TEST(FreeBlock) {
  physical_addr addr = alloc_block();
  free_block(addr);
  EXPECT_FALSE("Basic alloc-free test/1", is_alloced(addr));  
  return 0;
}

TEST(FreeAndAllocSameBlock) {
  physical_addr first_addr = alloc_block();
  free_block(first_addr); 
  EXPECT_FALSE("Free and alloc same block test/1", is_alloced(first_addr));
  physical_addr second_addr = alloc_block();
  EXPECT_TRUE("Free and alloc same block test/2", is_alloced(second_addr));  
  EXPECT_EQ("Free and alloc same block test/3", first_addr, second_addr);
  return 0;
}

TEST(AllocBlocks) {
  physical_addr first_block = alloc_blocks(5);
  EXPECT_TRUE("Multi-block allocation/1", first_block);
  for (int i = 0; i < 5; i++) {
    EXPECT_TRUE("Multi-block allocation/2",
      is_alloced(first_block + (PHYS_BLOCK_SIZE * i)));
  }
  return 0;
}

TEST(FreeBlocks) {
  physical_addr first_block = alloc_blocks(5);
  for (int i = 0; i < 5; i++) {
    EXPECT_TRUE("Multi-block freeing/1",
      is_alloced(first_block + (PHYS_BLOCK_SIZE * i)));
  }
  free_blocks(first_block, 5);
  for (int i = 0; i < 5; i++) {
    EXPECT_FALSE("Multi-block freeing/2",
      is_alloced(first_block + (PHYS_BLOCK_SIZE * i)));
  }
  return 0;
}

TEST(AllocTenFreeMiddleAndReallocMiddle) {
  physical_addr first_block = alloc_blocks(10);
  physical_addr fifth_block = first_block + (PHYS_BLOCK_SIZE * 4);
  free_blocks(fifth_block, 3);
  for (int i = 0; i < 3; i++) {
    EXPECT_FALSE("Alloc ten, free middle, realloc middle/1",
      is_alloced(fifth_block + (PHYS_BLOCK_SIZE * i)));
  }
  
  physical_addr new_alloc = alloc_blocks(3);
  for (int i = 0; i < 3; i++) {
    EXPECT_TRUE("Alloc ten, free middle, realloc middle/2",
      is_alloced(fifth_block + (PHYS_BLOCK_SIZE * i)));
    EXPECT_EQ("Alloc ten, free middle, realloc middle/3",
      new_alloc + (PHYS_BLOCK_SIZE * i),
      fifth_block + (PHYS_BLOCK_SIZE * i));
  }
  return 0;
}


TEST(AllocTenFreeMiddleAndReallocEnd) {
  physical_addr first_block = alloc_blocks(10);
  physical_addr fifth_block = first_block + (PHYS_BLOCK_SIZE * 4);
  free_blocks(fifth_block, 3);
  for (int i = 0; i < 3; i++) {
    EXPECT_FALSE("Alloc ten, free middle, realloc at the end/1",
      is_alloced(fifth_block + (PHYS_BLOCK_SIZE * i)));
  }
  physical_addr new_alloc = alloc_blocks(4);
  EXPECT_GT("Alloc ten, free middle, realloc at the end/2", 
    new_alloc, first_block + (PHYS_BLOCK_SIZE * 10));
  for (int i = 0; i < 3; i++) {
    EXPECT_TRUE("Alloc ten, free middle, realloc at the end/3",
      is_alloced(new_alloc + (PHYS_BLOCK_SIZE * i)));
  }
  return 0;
}

void test_phys_mem() {
  fn_ptr tests[7] = {AllocBlock, FreeBlock, FreeAndAllocSameBlock,
    AllocBlocks, FreeBlocks, AllocTenFreeMiddleAndReallocMiddle,
    AllocTenFreeMiddleAndReallocEnd
  };
  RUN_TESTS(tests, 7);
}


