#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include <asm.h>
#include <external/multiboot.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/irq.h>
#include <kernel/isrs.h>
#include <kernel/kb.h>
#include <kernel/timer.h>
#include <kernel/tty.h>
#include <libk/kphys_mem.h>

void kernel_early(struct multiboot_info* mb) {
  terminal_initialize();
  gdt_install();
  idt_install();
  isrs_install();
  irq_install();

  phys_memory_init(mb);
  
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
