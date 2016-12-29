#ifndef _LIBK_HEAP_
#define _LIBK_HEAP_

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <libk/memlayout.h>
#include <libk/virt_mem.h>

#define MALLOCED_CHECKSUM 0x1234u

#define META_ALLOC_SIZE sizeof(meta_alloc_t)

typedef struct meta_alloc_t {
	size_t size;
	struct meta_alloc_t* next;
	uint16_t checksum;
} meta_alloc_t;

typedef struct free_list_t {
  meta_alloc_t* head;
} free_list_t;

free_list_t free_list_;
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
static void untrack_memory_malloced() {
  memory_tracker_counter_ = NULL;
}

static void increase_memory_tracker(meta_alloc_t* block) {
  if (memory_tracker_counter_)
    memory_tracker_counter_ += block->size;
}

static void decrease_memory_tracker(meta_alloc_t* block) {
  if (memory_tracker_counter_)
    memory_tracker_counter_ -= block->size;
}

// Find the first free block that can fit the given size, NULL otherwise
// The block is removed from the given list
meta_alloc_t* first_free_block(free_list_t* free_list, size_t bytes);

// Given the metadata of an alloced block of size > than bytes, split the block
// into a new one with the requested size and resize the old one. The old one is
// added to the given FreeList. Returns the address of the new one
meta_alloc_t* split_block(meta_alloc_t* to_split, size_t bytes,
  free_list_t* free_list);

// Request 4KB of memory from the kernel on the given address
void request_memory(virtual_addr addr);

meta_alloc_t* get_metadata(virtual_addr addr);
void set_metadata(virtual_addr new_block, size_t bytes);

void kernel_heap_init();

#endif  // _LIBK_HEAP_