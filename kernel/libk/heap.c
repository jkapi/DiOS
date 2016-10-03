#include <string.h>
#include <libk/heap.h>

void kernel_heap_init() {
  for (int i = 0, virt_addr = HEAP_VIRT_ADDR_START; 
           i < HEAP_INITIAL_BLOCK_SIZE; 
           i++, virt_addr += 4096) {
  	alloc_page(virt_addr);
    printf("virt: %lx virt-to-phys: %lx \n",
      virt_addr, virt_to_phys(virt_addr));
  }

}