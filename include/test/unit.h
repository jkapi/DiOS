#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

typedef int (*fn_ptr)();

typedef struct test_info_t {
  char* name;
  size_t error_line_num;
  fn_ptr fn;
} test_info_t;

// Defines macros for testing
#define NEW_SUITE(suite_name, max_num_tests)         \
  void suite_name##_run() {                          \
    char* name = #suite_name;                        \
    test_info_t tests[max_num_tests];                \
    size_t test_count = 0;                           \

#define END_SUITE()                                  \
  size_t failed_tests = 0;                           \
  for (size_t i = 0; i < test_count; i++) {          \
    test_info_t* cur_test = &tests[i];               \
    if (cur_test->fn(cur_test) == -1) {              \
      failed_tests++;                                \
    }                                                \
  }                                                  \
  if (failed_tests == 0) {                           \
    printf("Test suite %s passed! %d/%d\n", name,    \
      test_count, test_count);                       \
  } else {                                           \
    printf("Test suite %s failed! %d/%d\n", name,    \
      failed_tests, test_count);                     \    
  }                                                  \
}                                                    \

#define RUN_SUITE(suite_name) do {                   \
  printf("Running suite " #suite_name "...\n");      \
  suite_name##_run();                                \
  printf("Suite done...\n");      \
} while (0)

#define TEST(fn_name)                                \
  auto int fn_name(test_info_t*);                    \
  tests[test_count].name = #fn_name;                 \
  tests[test_count++].fn = fn_name;                  \
  int fn_name(test_info_t* info)                     \

#define TEST_FAILED() {                              \
  printf("Test failed - %s. Found at %s:%d\n",       \
      info->name, __FILE__, __LINE__);               \
  return -1;                                         \
}                                                    

#define PASS() do {                                  \
  return 0;                                          \
} while (0)

#define EXPECT_EQ(expected, result) do {             \
  if (expected != result)                            \
    TEST_FAILED();                                   \
} while (0)

#define EXPECT_GT(expected, result) do {             \
  if (result >= expected)                            \
    TEST_FAILED();                                   \
} while (0)

#define EXPECT_LT(expected, result) do {             \
  if (result <= expected)                            \
    TEST_FAILED();                                   \
} while (0)

#define EXPECT_TRUE(expression) do {                 \
  if (!expression)                                   \
    TEST_FAILED();                                   \
} while (0)

#define EXPECT_FALSE(expression) do {                \
  if (expression)                                    \
    TEST_FAILED();                                   \
} while (0)