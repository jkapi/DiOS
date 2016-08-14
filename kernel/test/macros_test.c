#include <test/unit.h>
#include <test/macros_test.h>

TEST(BaseMacroTest) {
  return 0;
}

TEST(ExpectEqTest) {
  EXPECT_EQ("Should be equal", 5, 5);
  return 0;
}

TEST(ExpectEqTestFail) {
  EXPECT_EQ("Should be different", 5, 8);
  return 0;
}

TEST(ExpectTrue) {
  EXPECT_TRUE("Should be true", 5);
  return 0;
}

TEST(ExpectTrueFails) {
  EXPECT_TRUE("Should be false", 0);
  return 0;
}

void test_macros() {
  fn_ptr tests[5] = {BaseMacroTest, ExpectEqTest, ExpectEqTestFail,
    ExpectTrue, ExpectTrueFails};
  // This is the only test suite in the OS that should have failing tests
  RUN_TESTS(tests, 5);
}

