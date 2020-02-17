/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftmalloc_decl.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydzhuryn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/15 15:08:24 by ydzhuryn          #+#    #+#             */
/*   Updated: 2018/01/05 17:20:48 by ydzhuryn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FTMALLOC_DECL_H
# define FTMALLOC_DECL_H

# include <stdlib.h>

# ifdef _DEBUG
#  define FTMALLOC_DEBUG
# endif

#ifdef FTMALLOC_THREADSAFE
# ifndef WIN32
#  include <pthread.h>
#  define FTMALLOC_LOCK (pthread_mutex_lock(&g_ftmalloc_mutex))
#  define FTMALLOC_UNLOCK (pthread_mutex_unlock(&g_ftmalloc_mutex))
# else
#  define FTMALLOC_LOCK slwait(&g_ftmalloc_mutex)
#  define FTMALLOC_UNLOCK slrelease(&g_ftmalloc_mutex)
# endif
# else // FTMALLOC_THREADSAFE
#  define FTMALLOC_LOCK 0
#  define FTMALLOC_UNLOCK 0
#endif // FTMALLOC_THREADSAFE

# ifdef FTMALLOC_DEBUG
#  define FTMALLOC_DEBUG_ONLY(x) (x)
# else
#  define FTMALLOC_DEBUG_ONLY(x)
# endif

# ifdef FTMALLOC_DEBUG
#  include <assert.h>
#  define FTMALLOC_ASSERT(x) assert(x)
# else
#  define FTMALLOC_ASSERT(x) ((void)(x))
# endif

# define FTMALLOC_CHUNK_SZ (2 * sizeof(size_t))
# define FTMALLOC_MIN_SZ (sizeof(t_mem_chunk) - FTMALLOC_CHUNK_SZ)
# define FTMALLOC_CHUNK_SZ_OK(x) ((x) >= FTMALLOC_MIN_SZ && FTMALLOC_ALGN_OK(x))
# define FTMALLOC_BIN_HEADER_SZ (FTMALLOC_MEM_ALGN_UP(sizeof(t_mem_bin)))
# define FTMALLOC_BIN_SZ (2 * FTMALLOC_CHUNK_SZ + FTMALLOC_BIN_HEADER_SZ)

# define FTMALLOC_ALGN_UP(x, a) ((((size_t)(x)) + ((a) - 1)) & ~((a) - 1))

# define FTMALLOC_MEM_ALGN (2 * sizeof(size_t))
# define FTMALLOC_MEM_ALGN_UP(x) FTMALLOC_ALGN_UP(x, FTMALLOC_MEM_ALGN)
# define FTMALLOC_ALGN_OK(x) ((((size_t)(x)) & (FTMALLOC_MEM_ALGN - 1)) == 0)

# define FTMALLOC_PREV_IN_USE_BIT 1
# define FTMALLOC_SERVICE_BITS FTMALLOC_PREV_IN_USE_BIT

# define FTMALLOC_BIN_APPROX_MIN_NITEMS 128
# define FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_SMALL 128
# define FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_MEDIUM 1024

/*
** NOTE: when chunk is not freed .freed_prev and
** .freed_next are used for user data
*/
typedef struct s_mem_chunk	t_mem_chunk;
struct	s_mem_chunk
{
	size_t		prev_size;
	size_t		size;

	t_mem_chunk	*freed_prev;
	t_mem_chunk	*freed_next;
};

typedef struct s_mem_bin	t_mem_bin;
struct	s_mem_bin
{
	t_mem_chunk *head;
	t_mem_bin	*next;

	size_t		mem_user;
	size_t		mem_occupied;
	size_t		mem_allocated;
};

typedef struct s_mem	t_mem;
struct	s_mem
{
	t_mem_chunk *chunk;
	t_mem_bin	*bin;
};

struct	s_ftmalloc_state
{
	t_mem_bin		*bin_list_small;
	t_mem_bin		*bin_list_medium;
	t_mem_bin		*bin_list_large;

# ifdef FTMALLOC_DEBUG

	int				usage_alloc;
	int				usage_dealloc;
	int				usage_realloc;
	int				total_realloc_hits;
	int				total_mmap;
	int				total_munmap;
	size_t			total_alloc;
	size_t			total_alloc_copied;
	size_t			total_dealloc;

# endif

};

extern struct s_ftmalloc_state g_ftmalloc_state;

# ifdef FTMALLOC_THREADSAFE

#ifdef WIN32
extern int g_ftmalloc_mutex;
#else
extern pthread_mutex_t g_ftmalloc_mutex;
#endif

# endif

#endif
