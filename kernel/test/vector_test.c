#include <libk/vector.h>
#include <test/unit.h>

NEW_SUITE(VectorTest, 10);

TEST(IntVectorBasicUsage) {
  int_vector* vector = new_int_vector();
  push(vector, 1);
  int popped = pop(vector);
  EXPECT_EQ(1, popped);

  delete (vector);
}

TEST(CharPtrVectorBasicUsage) {
  char_ptr_vector* ptr_vector = new_char_ptr_vector(false);
  char c = 'a';
  push(ptr_vector, &c);
  char* popped = pop(ptr_vector);
  EXPECT_EQ(c, *popped);

  delete (ptr_vector);
}

TEST(VectorExpandsCorrectly) {
  char_vector* vector = new_char_vector();
  EXPECT_EQ(vector->size, 0);
  EXPECT_EQ(vector->capacity, 1);

  push(vector, 'a');
  EXPECT_EQ(vector->size, 1);
  EXPECT_EQ(vector->capacity, 1);

  push(vector, 'b');
  push(vector, 'c');
  EXPECT_EQ(vector->size, 3);
  EXPECT_EQ(vector->capacity, 4);

  push(vector, 'd');
  push(vector, 'e');
  EXPECT_EQ(vector->size, 5);
  EXPECT_EQ(vector->capacity, 8);

  for (int i = 4; i >= 0; i--) {
    char popped = pop(vector);
    EXPECT_EQ('a' + i, popped);
  }

  delete (vector);
}

END_SUITE();

void test_vector() { RUN_SUITE(VectorTest); }