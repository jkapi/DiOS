#ifndef _KERNEL_IDT_H_
#define _KERNEL_IDT_H_

// Sets up the IDT, should be called on early initialization
void idt_install();

#endif  // _KERNEL_IDT_H_