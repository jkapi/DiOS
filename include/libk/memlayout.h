// Defines constants to 

#define KERNEL_START_PADDR ((uint32_t) &kernel_phys_start)
#define KERNEL_END_PADDR ((uint32_t) &kernel_phys_end)
#define KERNEL_SIZE (KERNEL_START_PADDR - KERNEL_END_PADDR)

#define KERNEL_START_VADDR ((uint32_t) &kernel_virt_start)
#define KERNEL_END_VADDR ((uint32_t) &kernel_virt_end)

extern uint32_t kernel_phys_start;
extern uint32_t kernel_phys_end;
extern uint32_t kernel_virt_start;
extern uint32_t kernel_virt_end;

typedef uint32_t virtual_addr;
typedef uint32_t physical_addr;