#include "ftmalloc_windows_adaptor.h"
#include <windows.h>
#include <assert.h>
#include <stdio.h>

#ifdef WIN32

int slwait(int *sl)
{
	while (InterlockedCompareExchange((void **)sl, (void *)1, (void *)0) != 0)
		Sleep(0);
	return 0;
}

/* Release spin lock */
int slrelease(int *sl)
{
	InterlockedExchange(sl, 0);
	return 0;
}

/* getpagesize for windows */
long getpagesize(void)
{
	static long g_pagesize = 0;
	if (!g_pagesize)
	{
		SYSTEM_INFO system_info;
		GetSystemInfo(&system_info);
		g_pagesize = system_info.dwPageSize;
	}
	return g_pagesize;
}
static long getregionsize(void)
{
	static long g_regionsize = 0;
	if (!g_regionsize)
	{
		SYSTEM_INFO system_info;
		GetSystemInfo(&system_info);
		g_regionsize = system_info.dwAllocationGranularity;
	}
	return g_regionsize;
}

/* A region list entry */
typedef struct _region_list_entry
{
	void *top_allocated;
	void *top_committed;
	void *top_reserved;
	long reserve_size;
	struct _region_list_entry *previous;
} region_list_entry;

/* Allocate and link a region entry in the region list */
static int region_list_append(region_list_entry **last, void *base_reserved, long reserve_size)
{
	region_list_entry *next = HeapAlloc(GetProcessHeap(), 0, sizeof(region_list_entry));
	if (!next)
		return FALSE;
	next->top_allocated = (char *)base_reserved;
	next->top_committed = (char *)base_reserved;
	next->top_reserved = (char *)base_reserved + reserve_size;
	next->reserve_size = reserve_size;
	next->previous = *last;
	*last = next;
	return TRUE;
}
/* Free and unlink the last region entry from the region list */
static int region_list_remove(region_list_entry **last)
{
	region_list_entry *previous = (*last)->previous;
	if (!HeapFree(GetProcessHeap(), sizeof(region_list_entry), *last))
		return FALSE;
	*last = previous;
	return TRUE;
}

#define CEIL(size,to)	(((size)+(to)-1)&~((to)-1))
#define FLOOR(size,to)	((size)&~((to)-1))

/* mmap for windows */
void *mmap(void *ptr, long size, long prot, long type, long handle, long arg)
{
	static long g_pagesize;
	static long g_regionsize;

	/* First time initialization */
	if (!g_pagesize)
		g_pagesize = getpagesize();
	if (!g_regionsize)
		g_regionsize = getregionsize();
	/* Assert preconditions */
	assert((unsigned)ptr % g_regionsize == 0);
	assert(size % g_pagesize == 0);
	/* Allocate this */
	ptr = VirtualAlloc(ptr, size,
						MEM_RESERVE | MEM_COMMIT | MEM_TOP_DOWN, PAGE_READWRITE);
	if (!ptr)
	{
		ptr = (void *)MORECORE_FAILURE;
		goto mmap_exit;
	}
	/* Assert postconditions */
	assert((unsigned)ptr % g_regionsize == 0);
mmap_exit:
	return ptr;
}

/* munmap for windows */
long munmap(void *ptr, long size)
{
	static long g_pagesize;
	static long g_regionsize;
	int rc = -1;
	/* First time initialization */
	if (!g_pagesize)
		g_pagesize = getpagesize();
	if (!g_regionsize)
		g_regionsize = getregionsize();
	/* Assert preconditions */
	assert((unsigned)ptr % g_regionsize == 0);
	assert(size % g_pagesize == 0);
	/* Free this */
	if (!VirtualFree(ptr, 0,
		MEM_RELEASE))
		goto munmap_exit;
	rc = 0;
munmap_exit:
	return rc;
}

int setenv(const char *name, const char *value, int overwrite)
{
    _putenv_s(name, value);
}

int     unsetenv(const char *name)
{
    _putenv_s(name, "");
}

void    ft_putendl_fd(char const *s, int fd)
{
    if (fd == 1)
    {
        printf("%s\n", s);
    }
    else if (fd == 2)
    {
        fprintf(stderr, "%s\n", s);
    }
}

void    ft_putstr(const char* str)
{
	while (*str)
	{
		ft_putchar(*str++);
	}
}

void    ft_putchar(const char ch)
{
	putc(ch, stdout);
}

void    ft_putnbr(int n)
{
	if (n == -2147483648)
	{
		ft_putnbr(-21);
		ft_putnbr(47483648);
		return;
	}
	if (n < 0)
	{
		n = -n;
		putc('-', stdout);
	}
	if (n / 10 == 0)
	{
		putc(n + '0', stdout);
		return;
	}
	ft_putnbr(n / 10);
	putc(n % 10 + '0', stdout);
}

#define HEX "0123456789ABCDEF"

void ft_putaddress(void *addr)
{
	char	b[sizeof(void*) * 2 + 1];
	int		i;
	size_t	a;

	ft_memset(b, '0', sizeof(b));
	b[sizeof(b) - 1] = 0x0;
	a = *((size_t*)&addr);
	i = sizeof(b) - 1;
	while (a)
	{
		b[--i] = HEX[a % 16];
		a /= 16;
	}
	ft_putstr(b);
}

#endif /* WIN32 */
