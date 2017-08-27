#include <libk/heap.h>
#include <libk/types.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

string* new_string() {
  string* string = kcalloc(sizeof(string));
  string->contents = kcalloc(sizeof(char) * 1);
  return string;
}

string* new_string_2(char* contents, size_t size) {
  string* string = kcalloc(sizeof(string));
  string->size = size;
  string->contents = kcalloc(sizeof(char) * (size + 1));
  memcpy(string->contents, contents, size);
  return string;
}

void copy_string(string* to_copy, string* output) {
  output->size = to_copy->size;
  output->contents = kcalloc(sizeof(char) * (to_copy->size + 1));
  memcpy(output->contents, to_copy->contents, to_copy->size);
}

void delete_string(string* str) {
  kfree(str->contents);
  kfree(str);
}

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
  return str1->size == str2->size &&
         memcmp(str1->contents, str2->contents, str1->size) == 0;
}