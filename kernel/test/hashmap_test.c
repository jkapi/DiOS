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

TEST(IntToIntHashMapBasicUsage) {
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

TEST(IntToIntMultipleItemsWithCollisions) {
  int_to_int_hashmap* map = new_int_to_int_hashmap();
  int key;
  int value_found;
  add(map, 1, 10);
  add(map, DEFAULT_BUCKET_SIZE, 11);
  add(map, DEFAULT_BUCKET_SIZE * 2, 12);
  add(map, DEFAULT_BUCKET_SIZE * 3, 13);

  int key3 = DEFAULT_BUCKET_SIZE;
  EXPECT_TRUE(get(map, &key3, &value_found));
  EXPECT_EQ(11, value_found);

  key = DEFAULT_BUCKET_SIZE * 2;
  EXPECT_TRUE(get(map, &key, &value_found));
  EXPECT_EQ(12, value_found);

  key = DEFAULT_BUCKET_SIZE * 3;
  EXPECT_TRUE(get(map, &key, &value_found));
  EXPECT_EQ(13, value_found);

  int key2 = 1;
  EXPECT_TRUE(get(map, &key2, &value_found));
  EXPECT_EQ(10, value_found); 

  delete(map);
}

TEST(StringToStringHashMapBasicUsage) {
  string_to_string_hashmap* map = new_string_to_string_hashmap();
  string* key = new_string("key", 3);
  string* value = new_string("value", 5);
  add(map, *key, *value);

  string value_found;
  EXPECT_TRUE(get(map, key, &value_found));
  EXPECT_TRUE(string_equals(value, &value_found));

  delete(map);
  delete_string(key);
  delete_string(value);
}

END_SUITE();

void test_hashmap() { RUN_SUITE(HashMapTest); }