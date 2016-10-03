#ifndef _LIBK_HEAP_
#define _LIBK_HEAP_

#include <libk/memlayout.h>
#include <libk/phys_mem.h>
#include <libk/virt_mem.h>

void* kmalloc(size_t size);
void kfree(void* ptr);
void kernel_heap_init();

#endif  // _LIBK_HEAP_