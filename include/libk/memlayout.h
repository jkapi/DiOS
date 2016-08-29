#ifndef _LIBK_MEMLAYOUT_
#define _LIBK_MEMLAYOUT_

// Defines constants to 

#define KERNEL_START_PADDR ((uint32_t) &kernel_phys_start)
#define KERNEL_END_PADDR ((uint32_t) &kernel_phys_end)
#define KERNEL_SIZE (KERNEL_END_PADDR - KERNEL_START_PADDR)

#define KERNEL_START_VADDR ((uint32_t) &kernel_virt_start)
#define KERNEL_END_VADDR ((uint32_t) &kernel_virt_end)

#define KERNEL_PHYS_MAP_START kernel_phys_map_start
#define KERNEL_PHYS_MAP_END kernel_phys_map_end


// Addresses of the Kernel in the Physical and Virtual memory
extern uint32_t kernel_phys_start;
extern uint32_t kernel_phys_end;
extern uint32_t kernel_virt_start;
extern uint32_t kernel_virt_end;

// Addresses of the Physical Memory Map in the Physical memory
extern uint32_t kernel_phys_map_start;
extern uint32_t kernel_phys_map_end;

typedef uint32_t virtual_addr;
typedef uint32_t physical_addr;

#endif  // _LIBK_MEMLAYOUT_