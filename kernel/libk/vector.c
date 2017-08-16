#include <libk/vector.h>
#include <string.h>

vector* new_vector(size_t vector_size, size_t data_size) {
  vector* vector = kmalloc(vector_size);
  if (vector == NULL) {
    return NULL;
  }

  vector->size = 0;
  vector->capacity = DEFAULT_CAPACITY;
  vector->data_size = data_size;
  vector->data = kmalloc(data_size * DEFAULT_CAPACITY);
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

void vector_pop(vector* vector) {
  if (vector->size == 0) {
    // error
    return;
  }
}

void vector_resize(vector* vector) {
  // TODO(psamora) When kheap become better, realloc would be awesome
  uint32_t new_capacity = vector->capacity * 2;
  uint32_t memory_size = vector->data_size * new_capacity;
  void* new_data = kmalloc(memory_size);
  if (new_data == NULL) {
    // error;
  }

  memcpy(new_data, vector->data, memory_size);

  kfree(vector->data);
  vector->capacity = new_capacity;
  vector->data = new_data;
}