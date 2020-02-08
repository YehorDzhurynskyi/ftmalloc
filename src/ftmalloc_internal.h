/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftmalloc_internal.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydzhuryn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/15 15:08:24 by ydzhuryn          #+#    #+#             */
/*   Updated: 2018/01/05 17:20:48 by ydzhuryn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FTMALLOC_INTERNAL_H
# define FTMALLOC_INTERNAL_H

# include <stddef.h>
# include <errno.h>

#ifdef WIN32
# include "ftmalloc_windows_adaptor.h"
#else
# include "ft.h"
# include <sys/mman.h>
# include <unistd.h>
#endif

#ifdef _DEBUG
# define FTMALLOC_DEBUG
#endif

#ifdef FTMALLOC_THREADSAFE
# ifndef WIN32
#  include <pthread.h>
#  define FTMALLOC_LOCK (pthread_mutex_lock(&g_ftmalloc_mutex))
#  define FTMALLOC_UNLOCK (pthread_mutex_unlock(&g_ftmalloc_mutex))
# else
#  define FTMALLOC_LOCK slwait(&g_ftmalloc_mutex)
#  define FTMALLOC_UNLOCK slrelease(&g_ftmalloc_mutex)
# endif

#else
# define FTMALLOC_LOCK 0
# define FTMALLOC_UNLOCK
#endif

#ifdef FTMALLOC_DEBUG
# define FTMALLOC_DEBUG_ONLY(x) (x)
#else
# define FTMALLOC_DEBUG_ONLY(x)
#endif

#ifdef FTMALLOC_DEBUG
# include <assert.h>
# define FTMALLOC_ASSERT(x) assert(x)
#else
# define FTMALLOC_ASSERT(x)
#endif

# define FTMALLOC_MEM_CHUNK_SZ (2 * sizeof(size_t))
# define FTMALLOC_MEM_MIN_PAYLOAD_SZ (sizeof(t_mem_chunk) - FTMALLOC_MEM_CHUNK_SZ)
# define FTMALLOC_MEM_CHUNK_SZ_OK(x) ((x) >= FTMALLOC_MEM_MIN_PAYLOAD_SZ && FTMALLOC_MEM_ALIGNED_OK(x))
# define FTMALLOC_MEM_BIN_SZ (2 * FTMALLOC_MEM_CHUNK_SZ + FTMALLOC_MEM_ALIGN_UP(sizeof(t_mem_bin)))

# define FTMALLOC_ALIGN_UP(x, alignment) ((((size_t)(x)) + ((alignment) - 1)) & ~((alignment) - 1))

# define FTMALLOC_MEM_ALIGN (2 * sizeof(size_t))
# define FTMALLOC_MEM_ALIGN_UP(x) FTMALLOC_ALIGN_UP(x, FTMALLOC_MEM_ALIGN)
# define FTMALLOC_MEM_ALIGNED_OK(x) ((((size_t)(x)) & (FTMALLOC_MEM_ALIGN - 1)) == 0)

# define FTMALLOC_PREV_IN_USE_BIT 1
# define FTMALLOC_SERVICE_BITS FTMALLOC_PREV_IN_USE_BIT

# define FTMALLOC_BIN_APPROX_MIN_NITEMS 128
# define FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_SMALL 128
# define FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_MEDIUM 1024

/*
** NOTE: when chunk is not freed .freed_prev and .freed_next are used for user data
*/
typedef struct s_mem_chunk t_mem_chunk;
struct s_mem_chunk
{
	size_t		prev_size;
	size_t		size;

	t_mem_chunk	*freed_prev;
	t_mem_chunk	*freed_next;
};

typedef struct s_mem_bin t_mem_bin;
struct s_mem_bin
{
	t_mem_chunk *head;
	t_mem_bin   *next;

	size_t      mem_user;
	size_t      mem_occupied;
	size_t      mem_allocated;
};

typedef struct s_mem t_mem;
struct s_mem
{
	t_mem_chunk *chunk;
	t_mem_bin   *bin;
};

struct s_ftmalloc_state
{
	t_mem_bin		*bin_list_small;
	t_mem_bin		*bin_list_medium;
	t_mem_bin		*bin_list_large;

# ifdef FTMALLOC_DEBUG
	int	    		usage_alloc;
	int 			usage_dealloc;
	int             usage_realloc;
	int             total_realloc_hits;
	int	    		total_mmap;
	int 			total_munmap;
	size_t			total_alloc;
	size_t			total_alloc_copied;
	size_t			total_dealloc;
# endif
};

extern struct s_ftmalloc_state g_ftmalloc_state;

# ifdef FTMALLOC_THREADSAFE

#  ifdef WIN32
extern int g_ftmalloc_mutex;
#  else
extern pthread_mutex_t g_ftmalloc_mutex;
#  endif

# endif

/*
** INTERNAL GENERAL PURPOSE ALLOCATORS
*/
void		*ftmalloc_internal(size_t size);
void		*ftrealloc_internal(void *mem, size_t size);
void		ftfree_internal(void *mem);

/*
** DEBUG
*/
void		bin_verify_freed_links();
void		bin_verify(const t_mem_bin* bin);
void		chunk_verify(const t_mem_chunk* chunk);

/*
** SHOW MEM
*/
typedef void (*t_show_chunk_func)(const t_mem_chunk*);
void		show_mem_chunk(const t_mem_chunk *chunk);
void		show_mem_chunk_ex(const t_mem_chunk *chunk);

/*
** MEM
*/
size_t		mem_reserve(t_mem *mem, const size_t size);
size_t		mem_find(t_mem *mem, const size_t size);
size_t		mem_allocate(t_mem *mem, const size_t size);

/*
** BIN
*/
t_mem_bin	*bin_init(t_byte *mem, size_t size);

t_bool      bin_is_empty(const t_mem_bin *bin);

t_byte		*bin_bin2mem(t_mem_bin* bin);

t_mem_bin	*bin_list_of(size_t size);
size_t		bin_alloc_size_of(size_t size);
size_t      bin_max_size_of(size_t size);

t_mem_chunk *bin_adj(const t_mem_bin *bin);

/*
** BUDDY
*/
void        buddy_occupy(t_mem *mem, const size_t size);
void        buddy_deoccupy(t_mem* mem);
t_mem_chunk *buddy_try_merge_next(t_mem *mem);
t_mem_chunk *buddy_try_merge_prev(t_mem *mem);

/*
** CHUNK
*/
t_mem_chunk	*chunk_arrange(t_byte *mem, size_t size, t_bool inuse);

t_mem_bin   *chunk_bin_of(const t_mem_chunk *chunk, t_mem_bin **prev_bin);
t_mem_bin   *chunk_bin_of_slow(const t_mem_chunk *chunk);

t_bool      chunk_is_splittable(const size_t oldsize, const size_t newsize);

t_bool		chunk_is_prev_top(const t_mem_chunk* chunk);
t_bool		chunk_is_next_bottom(const t_mem_chunk* chunk);

t_byte		*chunk_chunk2mem(t_mem_chunk* chunk);
t_mem_chunk	*chunk_mem2chunk(t_byte* mem);

void		chunk_prev_in_use_set_true(t_mem_chunk* chunk);
void		chunk_prev_in_use_set_false(t_mem_chunk* chunk);
t_bool 		chunk_prev_in_use_get(const t_mem_chunk* chunk);

void		chunk_in_use_set_true(t_mem_chunk* chunk);
void		chunk_in_use_set_false(t_mem_chunk* chunk);
t_bool		chunk_in_use_get(const t_mem_chunk* chunk);

void		chunk_size_set(t_mem_chunk* chunk, size_t size);
size_t		chunk_size_get(const t_mem_chunk* chunk);

t_mem_chunk	*chunk_adj_prev(const t_mem_chunk* chunk);
t_mem_chunk	*chunk_adj_next(const t_mem_chunk* chunk);

t_mem_chunk	*chunk_freed_prev_get(const t_mem_chunk* chunk);
void		chunk_freed_prev_set(t_mem_chunk *chunk, t_mem_chunk *freed);
t_mem_chunk	*chunk_freed_next_get(const t_mem_chunk* chunk);
void		chunk_freed_next_set(t_mem_chunk *chunk, t_mem_chunk *freed);

/*
** REALLOC
*/
t_bool		realloc_try_grow(t_mem *mem, size_t size, size_t osize);
t_bool		realloc_try_shrink(t_mem *mem, size_t size, size_t osize);

#endif
