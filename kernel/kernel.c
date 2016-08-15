#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include <asm.h>
#include <devices/kb.h>
#include <devices/timer.h>
#include <external/multiboot.h>
#include <arch/i386/gdt.h>
#include <arch/i386/idt.h>
#include <arch/i386/irq.h>
#include <arch/i386/isrs.h>
#include <arch/i386/tty.h>
#include <libk/kphys_mem.h>
#include <libk/kvirt_mem.h>
#include <test/macros_test.h>
#include <test/phys_mem_test.h>

void kernel_early(struct multiboot_info* mb) {
  terminal_initialize();
  gdt_install();
  idt_install();
  isrs_install();
  irq_install();

  phys_memory_init(mb);
  // paging_install();

  timer_install();
  keyboard_install();
  enable_interrupts();

  test_macros();
  test_phys_mem();
}

void kernel_main(void) {
  printf("Hello, kernel World %d!\n", 25);
 for(;;) {
    asm("hlt");
 }
}
