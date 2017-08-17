#ifndef _LIBK_HEAP_
#define _LIBK_HEAP_

#include <libk/memlayout.h>
#include <libk/virt_mem.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define MALLOCED_CHECKSUM 0x12345678

#define HEAP_PAGE_ACTUAL_SIZE sizeof(heap_page_t)

typedef struct heap_page_t {
  // If equal to 0x12345678, this is an actual alloced heap page
  size_t checksum;
  // Number of available heap blocks. This doesn't guarantee that
  // this number of blocks are available in sequence.
  size_t num_available_blocks;
  // bitmap: 1 represents an alloc block, 0 a free block
  unsigned char alloced_block_bitmap[HEAP_BLOCK_BIT_MAP_SIZE];
  // bitmap: 1 represents the starting block of an allocation, else 0
  unsigned char first_alloced_bitmap[HEAP_BLOCK_BIT_MAP_SIZE];
  // Actual memory being referenced by the bitmaps
  unsigned char alloc_memory[HEAP_BLOCK_COUNT * HEAP_BLOCK_SIZE];
  // Next heap page in the heap page list
  struct heap_page_t* next;
} heap_page_t;

typedef struct heap_page_list_t {
  heap_page_t* head;
} heap_page_list_t;

heap_page_list_t heap_page_list_;
virtual_addr cur_heap_addr_;

void* kmalloc(size_t size);
void kfree(void* ptr);

// These are two functions use to compute memory leaks.
// Pass in the pointer to the counter to be used to track the leaks.
// While that counter is set, it will be populated with the malloced/freed bytes
// Once the tracking is done, the user MUST call untrack_memory_malloced
static unsigned long* memory_tracker_counter_ = NULL;

static void track_memory_malloced(unsigned long* counter) {
  memory_tracker_counter_ = counter;
}
static void untrack_memory_malloced() { memory_tracker_counter_ = NULL; }

static void increase_memory_tracker(size_t bytes) {
  if (memory_tracker_counter_) memory_tracker_counter_ += bytes;
}

static void decrease_memory_tracker(size_t bytes) {
  if (memory_tracker_counter_) memory_tracker_counter_ -= bytes;
}

void kernel_heap_init();
heap_page_t* get_heap_block_metadata(void* ptr);

#endif  // _LIBK_HEAP_