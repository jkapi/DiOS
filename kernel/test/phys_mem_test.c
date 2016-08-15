#include <test/unit.h>
#include <libk/kphys_mem.h>

TEST(AllocBlock) {
  void* addr = alloc_block();
  printf("%x", addr);
  EXPECT_TRUE("Basic allocation test/1", addr);
  EXPECT_TRUE("Basic allocation test/2", is_alloced(addr));
  return 0;
}

TEST(FreeBlock) {
  void* addr = alloc_block();
  free_block(addr);
  EXPECT_FALSE("Basic alloc-free test/1", is_alloced(addr));  
  return 0;
}

TEST(FreeAndAllocSameBlock) {
  void* first_addr = alloc_block();
  free_block(first_addr); 
  void* second_addr = alloc_block();
  printf("%x %x", first_addr, second_addr);
  EXPECT_FALSE("Free and alloc same block test/1", is_alloced(first_addr)); 
  EXPECT_TRUE("Free and alloc same block test/2", is_alloced(second_addr));  
  EXPECT_EQ("Free and alloc same block test/3", first_addr, second_addr);
  return 0;
}

void test_phys_mem() {
  fn_ptr tests[3] = {AllocBlock, FreeBlock, FreeAndAllocSameBlock};
  RUN_TESTS(tests, 3);
}


