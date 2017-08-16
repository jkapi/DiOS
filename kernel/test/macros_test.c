#include <test/macros_test.h>
#include <test/unit.h>

NEW_SUITE(MacrosTest, 5);

TEST(BaseMacroTest) {}

TEST(ExpectEqTest) { EXPECT_EQ(5, 5); }

TEST(ExpectEqTestFail) { EXPECT_EQ(5, 8); }

TEST(ExpectTrue) { EXPECT_TRUE(5); }

TEST(ExpectTrueFails) { EXPECT_TRUE(0); }

END_SUITE();

void test_macros() { RUN_SUITE(MacrosTest); }
