#include <stdio.h>

typedef char* (*fn_ptr)();

// Defines macros for testing

#define TEST(fn_name) static char* fn_name()

#define EXPECT_EQ(message, expected, result) do {    \
  if (expected != result)                            \
    return message;                                  \
} while (0)

#define EXPECT_TRUE(message, expression) do {        \
  if (!expression)                                   \
    return message;                                  \
} while (0)

#define EXPECT_FALSE(message, expression) do {       \
  if (expression)                                    \
    return message;                                  \
} while (0)

#define RUN_TESTS(tests_arr, size) do {              \
  int tests_failed = 0;                              \
  for (int i = 0; i < size; i++) {                   \
    char *message = tests_arr[i]();                  \
    if (message) {                                   \
      printf("Test failed! %s\n", message);          \
      tests_failed++;                                \
    }                                                \
  }                                                  \
  printf("\nTests run: %d, passed: %d\n", size,      \
    size - tests_failed);                            \
} while (0)