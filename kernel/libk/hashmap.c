#include <libk/hashmap.h>

void hashmap_resize(hashmap* hashmap) {
  return;
}

hashmap* new_hashmap(size_t vector_size, size_t entry_size) {
  hashmap* hashmap = kcalloc(vector_size);
  if (hashmap == NULL) {
    return NULL;
  }

  hashmap->size = 0;
  hashmap->capacity = DEFAULT_BUCKET_SIZE;
  hashmap->entry_size = entry_size;
  hashmap->buckets = kcalloc(sizeof(void*) * DEFAULT_BUCKET_SIZE);

  if (hashmap->buckets == NULL) {
    kfree(hashmap);
    return NULL;
  }
  return hashmap;
}