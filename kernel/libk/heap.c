#include <string.h>
#include <libk/heap.h>

void kernel_heap_init() {
  void* kernel_heap_start = ALIGN_BLOCK(kernel_max_virt_addr + 4096);
  for (int i = 0, virt_addr = kernel_heap_start; 
           i < HEAP_INITIAL_BLOCK_SIZE; 
           i++, virt_addr += 4096) {
    void* phys_addr = alloc_block();
    map_page(phys_addr, virt_addr);
    printf("phys: %lx virt: %lx virt-to-phys: %lx \n",
      phys_addr, virt_addr, virt_to_phys(virt_addr));
  }

}