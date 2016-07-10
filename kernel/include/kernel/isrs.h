#ifndef _KERNEL_ISRS_H_
#define _KERNEL_ISRS_H_

// Install the ISRS into the IDT, should be called on early initialization
void isrs_install();

#endif  // _KERNEL_ISRS_H_