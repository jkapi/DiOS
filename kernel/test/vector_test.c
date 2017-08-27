#include <libk/vector.h>
#include <test/unit.h>

NEW_SUITE(VectorTest, 7);

TEST(IntVectorCreateDestroy) {
  int_vector* vector = new_int_vector();
  delete(vector);
}

TEST(IntVectorBasicUsage) {
  int_vector* vector = new_int_vector();
  push(vector, 1);

  int output;
  bool popped = pop(vector, &output);
  EXPECT_TRUE(popped);
  EXPECT_EQ(1, output);

  delete(vector);
}

TEST(CharPtrVectorBasicUsage) {
  char_ptr_vector* ptr_vector = new_char_ptr_vector();
  char c = 'a';
  push(ptr_vector, &c);

  char* output;
  bool popped = pop(ptr_vector, &output);
  EXPECT_TRUE(popped);
  EXPECT_EQ(&c, output);

  delete(ptr_vector);
}

TEST(VectorExpandsCorrectly) {
  char_vector* vector = new_char_vector();
  EXPECT_EQ(0, vector->size);
  EXPECT_EQ(1, vector->capacity);

  push(vector, 'a');
  EXPECT_EQ(1, vector->size);
  EXPECT_EQ(1, vector->capacity);

  push(vector, 'b');
  push(vector, 'c');
  EXPECT_EQ(3, vector->size);
  EXPECT_EQ(4, vector->capacity);

  push(vector, 'd');
  push(vector, 'e');
  EXPECT_EQ(5, vector->size);
  EXPECT_EQ(8, vector->capacity);

  for (int i = 4; i >= 0; i--) {
    char output;
    bool popped = pop(vector, &output);
    EXPECT_TRUE(popped);
    EXPECT_EQ('a' + i, output);
  }

  delete(vector);
}

TEST(PopEmptyVectorFails) {
  int_vector* vector = new_int_vector();
  int output = 12345;
  bool popped = pop(vector, &output);
  EXPECT_FALSE(popped);
  EXPECT_EQ(12345, output);

  delete(vector);
}

TEST(VectorGet) {
  int_vector* vector = new_int_vector();
  push(vector, 1);
  push(vector, 2);

  int output;
  bool found = get(vector, 0, &output);
  EXPECT_TRUE(found);
  EXPECT_EQ(1, output);

  found = get(vector, 1, &output);
  EXPECT_TRUE(found);
  EXPECT_EQ(2, output);

  delete(vector);
}


TEST(VectorGetOutOfBoundsFail) {
  int_vector* vector = new_int_vector();
  int output = 12345;
  bool found = get(vector, 0, &output);
  EXPECT_FALSE(found);
  EXPECT_EQ(output, 12345);

  push(vector, 1);

  found = get(vector, 1, &output);
  EXPECT_FALSE(found);
  EXPECT_EQ(12345, output);
  delete(vector);
}


END_SUITE();

void test_vector() { RUN_SUITE(VectorTest); }