#include <stdio.h>

#if defined(__is_myos_kernel)
#include <kernel/tty.h>
#endif

int putchar(int ic) {
#if defined(__is_dios_kernel)
  char c = (char) ic;
  t_write(&c, sizeof(c));
#else
  // TODO: You need to implement a write system call.
#endif
  return ic;
}
