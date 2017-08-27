#include <libk/vector.h>
#include <stdio.h>
#include <string.h>

vector* new_vector(size_t vector_size) {
  vector* vector = kcalloc(vector_size);
  if (vector == NULL) {
    return NULL;
  }

  vector->size = 0;
  vector->capacity = DEFAULT_CAPACITY;
  vector->data = kcalloc(sizeof(void*) * DEFAULT_CAPACITY);
  if (vector->data == NULL) {
    kfree(vector);
    return NULL;
  }

  return vector;
}

void delete_vector(vector* vector) {
  kfree(vector->data);
  kfree(vector);
}

void vector_resize(vector* vector) {
  // TODO(psamora) When kheap become better, realloc would be awesome
  uint32_t new_capacity = vector->capacity * 2;

  void** new_data = kcalloc(sizeof(void*) * new_capacity);
  if (new_data == NULL) {
    // error;
  }

  for (size_t i = 0; i < vector->size; i++) {
    new_data[i] = vector->data[i];
  }

  kfree(vector->data);
  vector->capacity = new_capacity;
  vector->data = new_data;
}