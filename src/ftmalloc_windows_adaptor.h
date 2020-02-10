#ifndef MALLOC_WINDOWS_ADAPTOR_H
# define MALLOC_WINDOWS_ADAPTOR_H

#ifdef WIN32

#include <stdio.h>
#include <math.h>

void    *mmap(void *ptr, long size, long prot, long type, long handle, long arg);
long    munmap(void *ptr, long size);
long    getpagesize();
int     slwait(int *sl);
int     slrelease(int *sl);
int     setenv(const char *name, const char *value, int overwrite);
int     unsetenv(const char *name);

typedef int t_bool;
typedef unsigned char t_byte;

#define FALSE 0
#define TRUE 1

#define MORECORE_FAILURE ((void*)(-1))
#define MAP_FAILED MORECORE_FAILURE
#define MAP_PRIVATE 1
#define MAP_ANONYMOUS 2
#define PROT_READ 1
#define PROT_WRITE 2

#define ft_isprint isprint
#define ft_printf printf
#define ft_memcpy memcpy
#define ft_memset memset
#define ft_ceil ceil

#endif /* WIN32 */

#endif
