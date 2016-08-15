#ifndef _LIBK_KPHYS_MEM_H_
#define _LIBK_KPHYS_MEM_H_

#include <stdbool.h>
#include <stdint.h>

#define KERNEL_START_ADDR ((uint32_t) &kernel_start)
#define KERNEL_END_ADDR ((uint32_t) &kernel_end)
#define KERNEL_SIZE (KERNEL_START_ADDR - KERNEL_END_ADDR)

#define PHYS_BLOCKS_PER_BYTE 8
#define PHYS_BLOCK_SIZE 4096

typedef uint32_t physical_addr;

// Constants defined in the linker
extern uint32_t kernel_start;
extern uint32_t kernel_end;

// Physical memory manager
// Currently implemented using bit map based allocation
static uint32_t* phys_memory_map_ = 0;
static uint32_t phys_mem_size_kb_ = 0;
static uint32_t used_blocks_ = 0;
static uint32_t total_blocks_ = 0;

void phys_memory_init(struct multiboot_info* mb);

void* alloc_block();
void* allocate_blocks(uint32_t count);

void free_block(void* p);
void free_blocks(void* p, uint32_t count);

bool is_alloced(void* p);

#endif  // _LIBK_KPHYS_MEM_H_