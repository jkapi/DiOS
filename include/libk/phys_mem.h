#ifndef _LIBK_KPHYS_MEM_H_
#define _LIBK_KPHYS_MEM_H_

#include <stdbool.h>
#include <stdint.h>
#include <libk/memlayout.h>

// Physical memory manager
// Currently implemented using bit map based allocation
static uint32_t* phys_memory_map_ = 0;
static uint32_t phys_mem_size_kb_ = 0;
static uint32_t used_blocks_ = 0;
static uint32_t total_blocks_ = 0;

uint32_t kernel_phys_map_start;
uint32_t kernel_phys_map_end;

void phys_memory_init(struct multiboot_info* mb);

void update_map_addr(uint32_t* addr);

void* alloc_block();
void* alloc_blocks(uint32_t count);

void free_block(void* p);
void free_blocks(void* p, uint32_t count);

bool is_alloced(void* p);

#endif  // _LIBK_KPHYS_MEM_H_