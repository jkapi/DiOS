#include <libk/types.h>
#include <stdbool.h>
#include <stdint.h>

// string* new_string(char* contents, size_t size) {}
// void delete_string(string* str);

uint32_t string_hash(string* str) {
  unsigned long hash = 5381;
  char* contents = str->contents;
  int c;

  while ((c = *contents++)) {
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
  }   

  return hash;
}

bool string_equals(string* str1, string* str2) {
  return true;
}