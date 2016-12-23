#ifndef _LIBK_HEAP_
#define _LIBK_HEAP_

#include <stdbool.h>
#include <libk/memlayout.h>
#include <libk/phys_mem.h>
#include <libk/virt_mem.h>

#define META_ALLOC_SIZE sizeof(meta_alloc_t)

typedef struct meta_alloc_t {
	size_t size;
	alloct_t* next;
	uint16_t checksum;
} meta_alloc_t;

meta_alloc_t* free_list_head;
virtual_addr cur_heap_addr;

void* kmalloc(size_t size);
void kfree(void* ptr);

// Given the metadata of an alloced block of size > than bytes, reduces
// the size of the allocated block to be == to bytes, and creates
// a new memory block for the rest of the memory.
// Returns the address of the newly created block
meta_alloc_t* split_block(meta_alloc_t* to_split, size_t bytes);

void request_memory(void* addr);
void set_metadata(void* addr, size_t bytes);

meta_alloc_t get_metadata(void* ptr);

#endif  // _LIBK_HEAP_