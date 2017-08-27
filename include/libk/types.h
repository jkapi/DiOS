#ifndef _TYPES_H_
#define _TYPES_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Define alias for pointer types
typedef char* char_ptr;

// Define core types used by the OS
typedef struct string {
  // Owned by string
  char* contents;
  size_t size;
} string;

// void delete_string(string* str);
string* new_string(char* contents, size_t size);
uint32_t string_hash(string* str);
bool string_equals(string* str1, string* str2);

// Core functions that allow primitives to be used in the kernel data structures
#define GENERATE_FNS_FOR_PRIMITIVE(type)                                       \
  static uint32_t type##_hash(type* i) { return *i; }                          \
  static bool type##_equals(type* i1, type* i2) { return *i1 == *i2; }         \
  static type copy_##type(type i) { return i; }                                \
  /* Destructor for primitives does nothing since they are never constructed */\
  static void delete_##type(type* i) {};

GENERATE_FNS_FOR_PRIMITIVE(int);
GENERATE_FNS_FOR_PRIMITIVE(char);
GENERATE_FNS_FOR_PRIMITIVE(double);
GENERATE_FNS_FOR_PRIMITIVE(char_ptr);

#endif  // _TYPES_H_