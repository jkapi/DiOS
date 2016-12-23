#include <string.h>
#include <libk/heap.h>

void kernel_heap_init() {
	free_list_head = NULL;
	cur_heap_addr = HEAP_VIRT_ADDR_START;
}

// TODO(psamora) Make it work for > 4096, refactor
void* kmalloc(size_t bytes) {
	if (!free_list_head) {
		request_memory(cur_heap_addr, bytes);
		free_list_head = (meta_alloct_t*) cur_heap_addr;
	} 	

	meta_alloct_t* alloced_ptr = find_free_block(free_list_head, bytes);
	return alloced_ptr + 1;
}

meta_alloc_t* find_free_block(meta_alloc_t* free_list_head, size_t bytes) {
	while (free_list_head && free_list_head.size < bytes) {
		free_list_head = free_list_head.next;
	}

	if (free_list_head == NULL) {
		// allocate
	}

	if (free_list_head.size > bytes) {
		split_block(free_list_head, bytes);
	}

	return free_list_head;
}

void kfree(void* ptr) {
	if (!ptr) {
		return;
	}

	meta_alloct_t* metadata = get_metadata(ptr);
	if (metadata->checksum != 0x12345678) {
		return; // abort
	}

	metadata->next = free_list_head;
	free_list_head = metadata;
}

meta_alloc_t* split_block(meta_alloc_t* to_split, size_t bytes) {
	void* block_start = (void*) to_split;
	void* block_end = to_split + META_ALLOC_SIZE + bytes;

	meta_alloc_t* new_block = add_metadata(block_end, to_split.size - bytes);
	to_split.size = bytes;
  return new_block;
}

void request_memory(void* addr) {
	if (!alloc_page(cur_heap_addr)) {
		// abort
	}
	add_metadata(addr, PAGE_SIZE);
	cur_heap_addr += PAGE_SIZE;
}

meta_alloc_t get_metadata(void* ptr) {
	return (meta_alloc_t*) ptr - 1;
}

meta_alloc_t* add_metadata(void* addr, size_t bytes) {
	meta_alloc_t* metadata = (meta_alloc_t*) ptr;
	metadata->size = bytes;
	metadata->next = NULL;
	metadata->checksum = 0x12345678;
  return metadata;
}