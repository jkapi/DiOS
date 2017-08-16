#ifndef _STDIO_H
#define _STDIO_H 1

#include <sys/cdefs.h>

#ifdef __cplusplus
extern "C" {
#endif // ifdef __cplusplus

int printf(const char* __restrict,
           ...);
int putchar(int);
int puts(const char*);

#ifdef __cplusplus
}
#endif // ifdef __cplusplus

#endif // ifndef _STDIO_H
