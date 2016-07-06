#ifndef _KERNEL_GDT_H
#define _KERNEL_GDT_H

// Sets up the GDT, should be called on early initialization
void gdt_install();

#endif  // _KERNEL_GDT_H