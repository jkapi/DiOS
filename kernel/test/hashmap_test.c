#include <libk/hashmap.h>
#include <libk/types.h>
#include <test/unit.h>
#include <stdio.h>

NEW_SUITE(HashMapTest, 6);

TEST(HashMapGetMissingItem) {
  int_to_int_hashmap* map = new_int_to_int_hashmap();
  int key = 1;
  int value_found = 10;
  EXPECT_FALSE(get(map, &key, &value_found));
  EXPECT_EQ(10, value_found);
  delete(map);
}

TEST(HashMapBasicUsage) {
  int_to_int_hashmap* map = new_int_to_int_hashmap();
  add(map, 1, 10);

  int key = 1;
  int value_found;
  EXPECT_TRUE(get(map, &key, &value_found));
  EXPECT_EQ(10, value_found);

  // Vector keys are copied by the map, so changing key shouldn't work
  key = 2;
  EXPECT_FALSE(get(map, &key, &value_found));

  delete(map);
}

TEST(MultipleItemsWithCollisions) {
  int_to_int_hashmap* map = new_int_to_int_hashmap();
  add(map, 1, 10);
  add(map, DEFAULT_BUCKET_SIZE, 11);
  add(map, DEFAULT_BUCKET_SIZE * 1, 12);
  add(map, DEFAULT_BUCKET_SIZE * 2, 13);

  int key = DEFAULT_BUCKET_SIZE;
  int value_found;
  EXPECT_TRUE(get(map, &key, &value_found));
  EXPECT_EQ(11, value_found);

  key = DEFAULT_BUCKET_SIZE * 2;
  EXPECT_TRUE(get(map, &key, &value_found));
  EXPECT_EQ(13, value_found);

  key = 1;
  EXPECT_TRUE(get(map, &key, &value_found));
  EXPECT_EQ(10, value_found); 

  delete(map);
}

END_SUITE();

void test_hashmap() { RUN_SUITE(HashMapTest); }