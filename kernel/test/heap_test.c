#include <libk/heap.h>
#include <test/unit.h>

TEST(Malloc) {
  size_t size = sizeof(int) * 10;
  int* ptr = kmalloc(size);

  meta_alloc_t* metadata = get_metadata(ptr);
  EXPECT_EQ("Basic kmalloc test/1", metadata->size, size);
  EXPECT_EQ("Basic kmalloc test/2", metadata->checksum, MALLOCED_CHECKSUM);
  EXPECT_EQ("Basic kmalloc test/3", metadata->next, NULL);
  return 0;
}

TEST(FreeNull) {
  kfree(NULL);
  return 0;
}

TEST(FreeNotMallocedDoesntWork) {
  // TODO(psamora) When we add abort,  figure out how to test this
  char* ptr = kmalloc(1);
  meta_alloc_t* metadata = get_metadata(ptr);

  // Since we are not freeing the right pointer, nothing will happen
  kfree(ptr + 1);

  EXPECT_EQ("Free but not malloced test/1", metadata->size, 1);
  EXPECT_EQ("Free but not malloced test/2", metadata->checksum,
    MALLOCED_CHECKSUM);
  EXPECT_EQ("Free but not malloced test/3", metadata->next, NULL);
  return 0;
}

TEST(FreeMalloced) {
  char* ptr = kmalloc(25);
  meta_alloc_t* metadata = get_metadata(ptr);
  kfree(ptr);
  EXPECT_EQ("Malloc and free test/1", metadata->size, 1);
  EXPECT_EQ("Malloc and free test/2", metadata->checksum,
    MALLOCED_CHECKSUM);
  EXPECT_EQ("Malloc and free test/3", metadata->next, NULL);
  return 0;
}

void test_heap() {
  fn_ptr tests[3] = {
    Malloc, FreeNull, FreeNotMallocedDoesntWork
  };
  RUN_TESTS(tests, 3);
}