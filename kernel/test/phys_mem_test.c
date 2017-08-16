#include <libk/phys_mem.h>
#include <test/unit.h>

NEW_SUITE(PhysMemTest, 7);

TEST(AllocBlock) {
  physical_addr addr = alloc_block();
  EXPECT_TRUE(addr);
  EXPECT_TRUE(is_alloced(addr));
}

TEST(FreeBlock) {
  physical_addr addr = alloc_block();
  free_block(addr);
  EXPECT_FALSE(is_alloced(addr));
}

TEST(FreeAndAllocSameBlock) {
  physical_addr first_addr = alloc_block();
  free_block(first_addr);
  EXPECT_FALSE(is_alloced(first_addr));
  physical_addr second_addr = alloc_block();
  EXPECT_TRUE(is_alloced(second_addr));
  EXPECT_EQ(first_addr, second_addr);
}

TEST(AllocBlocks) {
  physical_addr first_block = alloc_blocks(5);
  EXPECT_TRUE(first_block);
  for (int i = 0; i < 5; i++) {
    EXPECT_TRUE(is_alloced(first_block + (PHYS_BLOCK_SIZE * i)));
  }
}

TEST(FreeBlocks) {
  physical_addr first_block = alloc_blocks(5);
  for (int i = 0; i < 5; i++) {
    EXPECT_TRUE(is_alloced(first_block + (PHYS_BLOCK_SIZE * i)));
  }
  free_blocks(first_block, 5);
  for (int i = 0; i < 5; i++) {
    EXPECT_FALSE(is_alloced(first_block + (PHYS_BLOCK_SIZE * i)));
  }
}

TEST(AllocTenFreeMiddleAndReallocMiddle) {
  physical_addr first_block = alloc_blocks(10);
  physical_addr fifth_block = first_block + (PHYS_BLOCK_SIZE * 4);
  free_blocks(fifth_block, 3);
  for (int i = 0; i < 3; i++) {
    EXPECT_FALSE(is_alloced(fifth_block + (PHYS_BLOCK_SIZE * i)));
  }

  physical_addr new_alloc = alloc_blocks(3);
  for (int i = 0; i < 3; i++) {
    EXPECT_TRUE(is_alloced(fifth_block + (PHYS_BLOCK_SIZE * i)));
    EXPECT_EQ(new_alloc + (PHYS_BLOCK_SIZE * i),
              fifth_block + (PHYS_BLOCK_SIZE * i));
  }
}

TEST(AllocTenFreeMiddleAndReallocEnd) {
  physical_addr first_block = alloc_blocks(10);
  physical_addr fifth_block = first_block + (PHYS_BLOCK_SIZE * 4);
  free_blocks(fifth_block, 3);
  for (int i = 0; i < 3; i++) {
    EXPECT_FALSE(is_alloced(fifth_block + (PHYS_BLOCK_SIZE * i)));
  }
  physical_addr new_alloc = alloc_blocks(4);
  EXPECT_EQ(new_alloc, first_block + (PHYS_BLOCK_SIZE * 10));
  for (int i = 0; i < 3; i++) {
    EXPECT_TRUE(is_alloced(new_alloc + (PHYS_BLOCK_SIZE * i)));
  }
}

END_SUITE();

void test_phys_mem() { RUN_SUITE(PhysMemTest); }