#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include <kernel/gdt.h>
#include <kernel/tty.h>

void kernel_early(void) {
  terminal_initialize();
  gdt_install();
}

void kernel_main(void) {
  printf("Hello, kernel World!\n");
  printf("UHUUUL %d %s %c", 10, "Aaaa", 'a');
}
