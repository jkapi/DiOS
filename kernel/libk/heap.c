#include <libk/heap.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

void request_memory();
void initialize_heap_page(heap_page_t* heap_page);
heap_page_t* get_fitting_heap_page(heap_page_list_t* heap_page_list,
                                   size_t blocks_to_alloc);
int32_t find_fitting_block_start(heap_page_t* heap_page,
                                 size_t blocks_to_alloc);
void allocate_blocks(heap_page_t* heap_page,
                     int32_t first_fitting_block,
                     size_t blocks_to_alloc);

inline static void* align_block(void* ptr) {
  return (void*) (((virtual_addr) ptr / HEAP_BLOCK_SIZE) * HEAP_BLOCK_SIZE);
}

inline static bool map_check(unsigned char* bitmap, size_t block) {
  return bitmap[block / 8] & (1 << (block % 8));
}

inline static void map_set(unsigned char* bitmap, size_t block) {
  bitmap[block / 8] |= (1 << (block % 8));
}

inline static void map_unset(unsigned char* bitmap, size_t block) {
  bitmap[block / 8] &= ~(1 << (block % 8));
}

void kernel_heap_init() {
  heap_page_list_.head = NULL;
  cur_heap_addr_ = HEAP_VIRT_ADDR_START;
  printf("Kernel heap installed.\n");
}

// void print_heap_page_list(heap_page_list_t* heap_page_list) {
//   printf("FreeList: ");
//   meta_alloc_t* cur = heap_page_list->head;
//   while (cur) {
//     printf("%lx:%u", (uint32_t)cur, cur->size);
//     printf(" -> ");
//     cur = cur->next;
//   }
//   printf(" NULL\n");
// }


// TODO(psamora) Make it work for > 4096, refactor
void* kmalloc(size_t bytes) {
  if (bytes == 0) {
    return NULL;
  }

  size_t blocks_to_alloc = HEAP_BLOCKS_NEED_FOR_N_BYTES(bytes);

  // Find the first heap page that can fit our requested memory
  heap_page_t* free_heap_page = get_fitting_heap_page(&heap_page_list_,
                                                      blocks_to_alloc);

  // If we can't find a heap page that might fit the bytes, request a new
  // block of 4KB and try mallocing on it
  if (!free_heap_page) {
    request_memory(cur_heap_addr_);
    return kmalloc(bytes);
  }

  // Tries to fiend a sequence of blocks that can fit the bytes in the heap
  // page. If we can't find it, request a new block of 4KB and try mallocing it
  int32_t first_fitting_block = find_fitting_block_start(free_heap_page,
                                                         blocks_to_alloc);
  if (first_fitting_block == -1) {
    request_memory(cur_heap_addr_);
    return kmalloc(bytes);    
  }

  // Success! Populate the bitmaps in the heap page to indicate that we 
  // allocated the memory
  allocate_blocks(free_heap_page, first_fitting_block, blocks_to_alloc);

  increase_memory_tracker(bytes);
  return &free_heap_page->alloc_memory[
      first_fitting_block * HEAP_BLOCK_SIZE];
}

void kfree(void* ptr) {
  if (!ptr) {
    return;
  }
  heap_page_t* heap_page = get_heap_block_metadata(ptr);

  // Checks if we are actually freeing a malloced heap block
  if (heap_page->checksum != MALLOCED_CHECKSUM) {
    // abort
    return;
  }

  // Get a pointer to the beginning of the block of this pointer
  void* block_ptr = align_block(ptr);

  // Get the block number for this pointer
  size_t block_num = 
    (block_ptr - (void*) heap_page->alloc_memory) / HEAP_BLOCK_SIZE;

  // Check in the first_alloced_bitmap if this is the start of the allocation 
  if (map_check(heap_page->first_alloced_bitmap, block_num) == false) {
    // abort
    return;
  }

  // Unset the first block in the first_alloced_bitmap since it won't be the
  // first anymore
  map_unset(heap_page->first_alloced_bitmap, block_num);

  // Free the allocation from the alloced_block_bitmap for all blocks
  // by using the first_alloced_bitmap to find the start of the next alloc
  size_t alloc_block_size = 0;
  for (size_t i = block_num; i < HEAP_BLOCK_COUNT; i++) {
    bool is_first = map_check(heap_page->first_alloced_bitmap, i);
    bool is_alloced = map_check(heap_page->alloced_block_bitmap, i);
    if (is_first) {
      // We found the start of the next alloc, stop.
      break;
    }
    if (!is_alloced) {
      // We found memory that isn't allocated, stop.
      break;
    }
    map_unset(heap_page->alloced_block_bitmap, i);
    alloc_block_size++;
  }

  heap_page->num_available_blocks += alloc_block_size;
  decrease_memory_tracker(alloc_block_size * HEAP_BLOCK_SIZE);
}

// Requests 4KB from the virtual memory to be owned by the heap
void request_memory() {
  heap_page_t* new_heap_page = (heap_page_t*) cur_heap_addr_;
  heap_page_list_.head = new_heap_page;
  if (!alloc_page(cur_heap_addr_)) {
    // abort
    return;
  }
  initialize_heap_page(new_heap_page);
  cur_heap_addr_ += PAGE_SIZE;
}

void initialize_heap_page(heap_page_t* heap_page) {
  heap_page->checksum = MALLOCED_CHECKSUM;
  heap_page->num_available_blocks = HEAP_BLOCK_COUNT;
  // TODO(psamora) Mark last bits of bitmap as used since these aren't
  // actually memory blocks that we can free
}

// Returns the first existing heap page in the heap_page_list that can fit the
// given number of bytes. If none can be found, returns NULL
heap_page_t* get_fitting_heap_page(heap_page_list_t* heap_page_list,
                                   size_t blocks_to_alloc) {
  // If list is empty, just return NULL
  if (heap_page_list->head == NULL) {
    return NULL;
  }

  heap_page_t* cur = heap_page_list->head;

  while (cur && cur->num_available_blocks < blocks_to_alloc) {
    cur = cur->next;
  }

  return cur;
}

// Given a Heap Page and the number of blocks_to_alloc, return the Heap
// Block number of the first of a sequence of free blocks that can
// fit blocks_to_alloc. Returns -1 if no sequence exists.
int32_t find_fitting_block_start(heap_page_t* heap_page,
                                 size_t blocks_to_alloc) {
  size_t starting_block = 0;
  size_t starting_block_bit = 0;
  size_t cur_block_num = 0;
  for (size_t i = 0; i < HEAP_BLOCK_BIT_MAP_SIZE; i++) {
    unsigned char cur_byte = heap_page->alloced_block_bitmap[i];
    if (cur_byte == 0xFF) {
      cur_block_num = 0;
      continue;
    }

    for (size_t j = 0; j < 8; j++) {
      int bit = 1 << j;
      if (bit & cur_byte) {  // bit is set
        cur_block_num = 0;
        continue;
      }

      if (cur_block_num == 0) {
        starting_block = i;
        starting_block_bit = j;
      }
      cur_block_num += 1;

      if (cur_block_num == blocks_to_alloc) {
        // We found a sequence that can fit, return the beginning block number
        return (HEAP_BLOCK_BIT_MAP_SIZE * starting_block) + starting_block_bit;
      }
    }
  }
  return -1;
}

void allocate_blocks(heap_page_t* heap_page,
                     int32_t first_fitting_block,
                     size_t blocks_to_alloc) {
  // Mark the beginning of the block in the first_alloced_bitmap
  map_set(heap_page->first_alloced_bitmap, first_fitting_block);

  // Mark all the alloced blocks in the alloced_block_bitmap
  for (size_t i = first_fitting_block; 
       i < first_fitting_block + blocks_to_alloc;
       i++) {
    map_set(heap_page->alloced_block_bitmap, i);
  }
  // Update the num_available_blocks
  heap_page->num_available_blocks -= blocks_to_alloc;
}

heap_page_t* get_heap_block_metadata(void* ptr) {
  return (heap_page_t*) (((virtual_addr) ptr / PAGE_SIZE) * PAGE_SIZE);
}