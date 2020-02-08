/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftmalloc_malloc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydzhuryn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/15 15:08:24 by ydzhuryn          #+#    #+#             */
/*   Updated: 2018/01/05 17:20:48 by ydzhuryn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftmalloc_internal.h"

struct s_ftmalloc_state g_ftmalloc_state;

#ifdef WIN32
int g_ftmalloc_mutex = 0;
#else
pthread_mutex_t g_ftmalloc_mutex = PTHREAD_MUTEX_INITIALIZER;
#endif

static t_bool	_size_request_is_out_of_range(size_t size)
{
	size_t minsize;

	minsize = FTMALLOC_MEM_ALIGN_UP(FTMALLOC_MEM_CHUNK_SZ + FTMALLOC_MEM_MIN_PAYLOAD_SZ);
	return (size >= (size_t)(-2 * minsize));
}

void		*ftmalloc_internal(size_t size)
{
    t_mem mem;

    mem = (t_mem){ .chunk = NULL, .bin = NULL };
    FTMALLOC_ASSERT(FTMALLOC_MEM_ALIGNED_OK(FTMALLOC_MEM_CHUNK_SZ));
	FTMALLOC_ASSERT(FTMALLOC_MEM_ALIGNED_OK(FTMALLOC_MEM_MIN_PAYLOAD_SZ));
	if (size == 0)
	{
		return (NULL);
	}

	if (_size_request_is_out_of_range(size))
	{
		errno = ENOMEM;
		return (NULL);
	}
	size = FTMALLOC_MEM_ALIGN_UP(size) + FTMALLOC_MEM_CHUNK_SZ;
    if ((size = _reserve_mem(&mem, size)) == 0)
    {
		errno = ENOMEM;
        return (NULL);
    }

    _bin_verify(mem.bin);
    _chunk_verify(mem.chunk);

    _chunk_occupy(&mem, size);

    _bin_verify(mem.bin);
    _chunk_verify(mem.chunk);

    mem.bin->mem_user += _chunk_size_get(mem.chunk);
    mem.bin->mem_occupied += _chunk_size_get(mem.chunk);

#ifdef FTMALLOC_DEBUG
    g_ftmalloc_state.usage_alloc++;
    g_ftmalloc_state.total_alloc += _chunk_size_get(mem.chunk);
#endif

    FTMALLOC_ASSERT(mem.bin == _chunk_bin_of(mem.chunk, NULL) && mem.bin == _chunk_bin_of_slow(mem.chunk));
    FTMALLOC_ASSERT(FTMALLOC_MEM_ALIGNED_OK(_chunk_chunk2mem(mem.chunk)));

	return (_chunk_chunk2mem(mem.chunk));
}

void			*ftmalloc(size_t size)
{
    void *mem;

    if (FTMALLOC_LOCK != 0)
    {
        return (NULL);
    }

    mem = ftmalloc_internal(size);

    _ftmalloc_verify_freed_links();

    FTMALLOC_UNLOCK;

    return (mem);
}
