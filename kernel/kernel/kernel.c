#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include <asm.h>
#include <multiboot.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/irq.h>
#include <kernel/isrs.h>
#include <kernel/kb.h>
#include <kernel/timer.h>
#include <kernel/tty.h>

void kernel_early(uint32_t magic, struct multiboot_info* mb) {
  terminal_initialize();
  printf("Magic number: %lx\n", magic);
  multiboot_memory_map_t* mm = (multiboot_memory_map_t*) mb->mmap_addr;
  while ((unsigned int) mm < mb->mmap_addr + mb->mmap_length) {
    if (mm->type != MULTIBOOT_MEMORY_AVAILABLE) {
      printf("Unavailable physical address: %llx, type: %u, length: %llx\n",
        mm->addr, mm->type, mm->len);
    } else {
      printf("Available physical address: %llx, length: %llx\n",
        mm->addr, mm->len);      
    }
    mm = (multiboot_memory_map_t*) ((unsigned int) mm +
      mm->size + sizeof(mm->size));
  }

  gdt_install();
  idt_install();
  isrs_install();
  irq_install();
  timer_install();
  keyboard_install();
  enable_interrupts();
}

void kernel_main(void) {
  printf("Hello, kernel World %d!\n", 25);
 for(;;) {
    asm("hlt");
 }
}
