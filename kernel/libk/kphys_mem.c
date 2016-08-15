#include <stdio.h>
#include <string.h>

#include <external/multiboot.h>
#include <libk/kphys_mem.h>

// Functions to manipulate the bitmap

inline static void map_set(int bit) {
  phys_memory_map_[bit / 32] |= (1 << (bit % 32));
}

inline static void map_unset(int bit) {
  phys_memory_map_[bit / 32] &= ~(1 << (bit % 32));
}

inline static bool map_test(int bit) {
  return phys_memory_map_[bit / 32] & (1 << (bit % 32));
}

int find_free_block() {
  for (uint32_t i = 0; i < total_blocks_ / 32; i++) {
    uint32_t block = phys_memory_map_[i];
    if (block != 0xFFFFFFFF) {
      for (uint8_t j = 0; j < 32; j++) {
        if ((1 << j) & block) {
          return (32 * i) + j;
        }
      }
    }
  }
  return -1;
}

int find_free_blocks(uint32_t count) {
  uint32_t starting_block = 0;
  uint32_t starting_block_bit = 0;
  uint32_t cur_block_num = 0;
  for (uint32_t i = 0; i < total_blocks_ / 32; i++) {
    uint32_t cur_block = phys_memory_map_[i];
    if (cur_block != 0xFFFFFFFF) {
      if (!starting_block) starting_block = cur_block;
      for (uint8_t j = 0; j < 32; j++) {
        if ((1 << j) & cur_block) {
          if (!starting_block_bit) starting_block_bit = j;
          cur_block_num += 1;
          if (cur_block_num == count) {
            return (32 * starting_block) + starting_block_bit; 
          }
        } else {
          starting_block = 0;
          starting_block_bit = 0;
          cur_block_num = 0;          
        }
      }
    }
  } 
  return -1;
}

// Functions to manage a single block in memory
void* alloc_block() {
  if (total_blocks_ - used_blocks_ <= 0) {
    return 0;
  }

  int free_block = find_free_block();
  printf("Free block found %d\n", free_block);
  if (free_block == -1) {
    return 0;
  }

  map_set(free_block);
  uint32_t addr = free_block * PHYS_BLOCK_SIZE;
  used_blocks_++;
  printf("Allocated: %x\n", addr);
  return (void*) addr;
}

void free_block(void* p) {
  uint32_t addr = (uint32_t) p;
  int block = addr / PHYS_BLOCK_SIZE;
 
  map_unset(block);
  used_blocks_--;
}

bool is_alloced(void* p) {
  uint32_t addr = (uint32_t) p;
  int block = addr / PHYS_BLOCK_SIZE;
  return map_test(block);
}

// Functions to allocate multiple blocks of memory

void* allocate_blocks(uint32_t count) {
  if (total_blocks_ - used_blocks_ <= 0) {
    return 0;
  }

  int free_block = find_free_blocks(count);
  if (free_block == -1) {
    return 0;
  }

  for (uint32_t i = 0; i < count; i++) {
    map_set(free_block + i);
  }

  uint32_t addr = free_block * PHYS_BLOCK_SIZE;
  used_blocks_ += count;
  return (void*) addr;
}

void free_blocks(void* p, uint32_t count) {
  uint32_t addr = (uint32_t) p;
  int block = addr / PHYS_BLOCK_SIZE;

  for (uint32_t i = 0; i < count; i++)
    map_unset(block + i);

  used_blocks_ -= count;
}

// Internal functions to allocate ranges of memory

void allocate_chunk(int base_addr, int length) {
  int cur_block_addr = base_addr / PHYS_BLOCK_SIZE;
  int num_blocks = length / PHYS_BLOCK_SIZE;
  while (num_blocks-- >= 0) {
    map_set(cur_block_addr++);
    used_blocks_--;
  }
}

void free_chunk(int base_addr, int length) {
  int cur_block_addr = base_addr / PHYS_BLOCK_SIZE;
  int num_blocks = length / PHYS_BLOCK_SIZE;

  while (num_blocks--) {
    map_unset(cur_block_addr++);
    used_blocks_--;
  }
}

// Functions to initialize the Physical Memory Manager

void free_available_memory(struct multiboot_info* mb) {
  multiboot_memory_map_t* mm = (multiboot_memory_map_t*) mb->mmap_addr;
  while ((unsigned int) mm < mb->mmap_addr + mb->mmap_length) {
    if (mm->type == MULTIBOOT_MEMORY_AVAILABLE) {
      free_chunk(mm->addr, mm->len);
    }
    mm = (multiboot_memory_map_t*) ((unsigned int) mm +
      mm->size + sizeof(mm->size));
  }
  map_set(0);
}

void phys_memory_init(struct multiboot_info* mb) {
  phys_mem_size_kb_ = mb->mem_upper + mb->mem_lower;
  total_blocks_ = (phys_mem_size_kb_ * 1024) / PHYS_BLOCK_SIZE;
  used_blocks_ = total_blocks_;
  phys_memory_map_ = (uint32_t*) KERNEL_END_ADDR;
  memset(phys_memory_map_, 0xFF, total_blocks_ / PHYS_BLOCKS_PER_BYTE);

  // Frees memory GRUB considers available
  free_available_memory(mb);

  // From the freed memory, we need to allocate the ones used by the Kernel
  allocate_chunk(KERNEL_START_ADDR, KERNEL_SIZE);

  // We also need to allocate the memory used by the Physical Map itself
  allocate_chunk(*phys_memory_map_, total_blocks_);
  printf("PhysMem Manager installed. %lxKB, %lx blocks, %lx free blocks.\n",
    phys_mem_size_kb_, total_blocks_, total_blocks_ - used_blocks_);
}

