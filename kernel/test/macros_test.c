#include <test/unit.h>
#include <test/macros_test.h>

NEW_SUITE(MacrosTest, 5);

TEST(BaseMacroTest) {
  PASS();
}

TEST(ExpectEqTest) {
  EXPECT_EQ(5, 5);
  PASS();
}

TEST(ExpectEqTestFail) {
  EXPECT_EQ(5, 8);
  PASS();
}

TEST(ExpectTrue) {
  EXPECT_TRUE(5);
  PASS();
}

TEST(ExpectTrueFails) {
  EXPECT_TRUE(0);
  PASS();
}

END_SUITE();

void test_macros() {
  RUN_SUITE(MacrosTest);
}

