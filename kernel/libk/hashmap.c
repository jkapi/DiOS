#include <libk/hashmap.h>
#include <stdio.h>

void hashmap_resize(hashmap* hashmap) {
  return;
}

hashmap* new_hashmap(size_t hashmap_size) {
  hashmap* hashmap = kcalloc(hashmap_size);
  if (hashmap == NULL) {
    return NULL;
  }

  hashmap->size = 0;
  hashmap->capacity = DEFAULT_BUCKET_SIZE;
  hashmap->buckets = kcalloc(sizeof(void*) * DEFAULT_BUCKET_SIZE);

  if (hashmap->buckets == NULL) {
    kfree(hashmap);
    return NULL;
  }
  return hashmap;
}