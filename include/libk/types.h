#ifndef _TYPES_H_
#define _TYPES_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Define alias for pointer types
typedef char* char_ptr;

// Define core types used by the OS

// -- string -----------------------------
// A struct that holds and owns a char*. This struct should be treated as
// immutable, mutating any of its fields will lead to undefined behavior.
// Contents are guaranteed to be NULL terminated if constructors are used.
typedef struct string {
  char* contents;
  size_t size;
} string;

string* new_string();
string* new_string_2(char* contents, size_t size);
void copy_string(string* to_copy, string* output);
void delete_string(string* str);

uint32_t string_hash(string* str);
bool string_equals(string* str1, string* str2);
// -- string -----------------------------

// Core functions that allow primitives to be used in the kernel data structures
// A delete function should only be called for one of these if it known that
// the type was allocated in the heap.
#define GENERATE_FNS_FOR_PRIMITIVE(type)                                       \
  static type* new_##type() { return kmalloc(sizeof(type)); };                 \
  static uint32_t type##_hash(type* i) { return *i; }                          \
  static bool type##_equals(type* i1, type* i2) { return *i1 == *i2; }         \
  static void copy_##type(type* i, type* output) { *output = *i; }             \
  static void delete_##type(type* i) { kfree(i); };

GENERATE_FNS_FOR_PRIMITIVE(int);
GENERATE_FNS_FOR_PRIMITIVE(char);
GENERATE_FNS_FOR_PRIMITIVE(double);
GENERATE_FNS_FOR_PRIMITIVE(char_ptr);

#endif  // _TYPES_H_