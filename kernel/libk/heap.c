#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <libk/heap.h>

void kernel_heap_init() {
	free_list_.head = NULL;
	cur_heap_addr_ = HEAP_VIRT_ADDR_START;
	printf("Kernel heap installed.\n");
}

// TODO(psamora) Make it work for > 4096, refactor
void* kmalloc(size_t bytes) {
	// Find the first block that can fit our requested memory
	meta_alloc_t* free_block = first_free_block(&free_list_, bytes);

	// If we can't find a block, request 4KB to the heap and retry
	if (!free_block) {
		free_list_.head = (meta_alloc_t*) cur_heap_addr_;
		request_memory(cur_heap_addr_);
		return kmalloc(bytes);
	}

	meta_alloc_t* alloced_ptr = free_block;

	// If our block is bigger than the number of bytes we need, split it
	if (alloced_ptr->size > bytes) {
		alloced_ptr = split_block(free_block, bytes, &free_list_);
	}
	return alloced_ptr + 1; 
}

void kfree(void* ptr) {
	if (!ptr) {
		return;
	}

	meta_alloc_t* metadata = get_metadata((virtual_addr) ptr);

	// Checks if we are actually freeing a malloced pointer
	if (metadata->checksum != MALLOCED_CHECKSUM) {
		// abort
		return;
	}

	// Adds freed blocl to the head of the FreeList
	metadata->checksum = 0;
	metadata->next = free_list_.head;
	free_list_.head = metadata;
}

meta_alloc_t* first_free_block(free_list_t* free_list, size_t bytes) {
	// If list is empty, just return NULL
	if (free_list->head == NULL) {
		return NULL;
	}

	meta_alloc_t* prev = NULL;
	meta_alloc_t* cur = free_list->head;

	while (cur && cur->size < bytes) {
		prev = cur;
		cur = cur->next;
	}

	if (prev == NULL) {
		// If prev is NULL, the list only had one element and is now empty
		free_list->head = NULL;
	} else {
		// Otherwise, just remove the found block from the list
		prev->next = cur->next;
	}
	return cur;
}

meta_alloc_t* split_block(meta_alloc_t* old_block, size_t bytes,
		free_list_t* free_list) {
	// Creates and fill the metadata of the block of requested size
	size_t block_size = META_ALLOC_SIZE + bytes;
	meta_alloc_t* new_block = old_block + block_size;
	set_metadata((virtual_addr) new_block, bytes);
	
	// Resizes the old_block and attaches it to the front of the FreeList
	old_block->size = old_block->size - block_size;
	old_block->next = free_list->head;
	free_list->head = old_block;

	// Returns the newly created block addr
  return new_block;
}

void request_memory(virtual_addr addr) {
	if (!alloc_page(addr)) {
		// abort
		return;
	}
	set_metadata(addr, PAGE_SIZE);
	cur_heap_addr_ += PAGE_SIZE;
}

meta_alloc_t* get_metadata(virtual_addr addr) {
	return (meta_alloc_t*) addr - 1;
}

void set_metadata(virtual_addr addr, size_t bytes) {
	meta_alloc_t* metadata = (meta_alloc_t*) addr;
	metadata->size = bytes;
	metadata->next = NULL;
	metadata->checksum = MALLOCED_CHECKSUM;
}