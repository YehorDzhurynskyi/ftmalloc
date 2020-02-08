/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftmalloc_reserve_room.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydzhuryn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/15 15:08:24 by ydzhuryn          #+#    #+#             */
/*   Updated: 2018/01/05 17:20:48 by ydzhuryn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftmalloc_internal.h"

static size_t _handle_bucket_overflow(t_mem *mem, const size_t size)
{
    const size_t oldsize = _chunk_size_get(mem->chunk);
    const size_t newsize = size - FTMALLOC_MEM_CHUNK_SZ;

    if (oldsize != newsize && !_chunk_is_splittable(oldsize, newsize))
    {
        FTMALLOC_ASSERT(oldsize - newsize == FTMALLOC_MEM_MIN_PAYLOAD_SZ);

        if (oldsize + FTMALLOC_MEM_CHUNK_SZ > _bin_max_size_of(size))
        {
            return (_reserve_mem(mem, size + FTMALLOC_MEM_CHUNK_SZ));
        }
    }

    return (size);
}

static size_t _find_mem(t_mem *mem, const size_t size)
{
    t_mem_bin	*bin;
    t_mem_chunk	*chunk;

	FTMALLOC_ASSERT(FTMALLOC_MEM_CHUNK_SZ_OK(size));

    bin = _bin_list_of(size);
    while (bin != NULL)
    {
        _bin_verify(bin);
        if (bin->mem_allocated - bin->mem_occupied >= size)
        {
            chunk = bin->head;
            while (chunk != NULL)
            {
                _chunk_verify(chunk);
                if (_chunk_size_get(chunk) >= size - FTMALLOC_MEM_CHUNK_SZ)
                {
                    mem->chunk = chunk;
                    mem->bin = bin;
                    return (_handle_bucket_overflow(mem, size));
                }
                chunk = _chunk_freed_next_get(chunk);
            }
        }
        bin = bin->next;
    }

    return (0);
}

static size_t _allocate_mem(t_mem *mem, const size_t size)
{
	t_mem_bin	*bin_list;
    t_mem_bin	*bin;
    t_byte		*raw;
	size_t		alloc_size;

	alloc_size = _bin_alloc_size_of(size);

	raw = mmap(NULL,
	alloc_size,
	PROT_READ | PROT_WRITE,
	MAP_PRIVATE | MAP_ANONYMOUS,
	-1,
	0);

	if (raw == MAP_FAILED)
	{
		return (0);
	}

    FTMALLOC_DEBUG_ONLY(g_ftmalloc_state.total_mmap++);

#ifdef FTMALLOC_DEBUG
    for (unsigned int j = 0; j < alloc_size; ++j)
    {
        raw[j] = (char)(rand() % 256);
    }
#endif

	FTMALLOC_ASSERT(FTMALLOC_MEM_ALIGNED_OK(raw));
	bin = _init_mem_bin(raw, alloc_size);

	bin_list = _bin_list_of(size);
	if (bin_list != NULL)
	{
		while (bin_list->next) bin_list = bin_list->next;
		bin_list->next = bin;
	}
	else
	{
		if (size <= FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_SMALL)
		{
			g_ftmalloc_state.bin_list_small = bin;
		}
		else if (size <= FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_MEDIUM)
		{
			g_ftmalloc_state.bin_list_medium = bin;
		}
		else
		{
			g_ftmalloc_state.bin_list_large = bin;
		}
	}

    mem->bin = bin;
    mem->chunk = bin->head;
	return (size);
}

size_t _reserve_mem(t_mem *mem, const size_t size)
{
    size_t newsize;

    FTMALLOC_ASSERT(size > 0 && FTMALLOC_MEM_ALIGNED_OK(size));

    if ((newsize = _find_mem(mem, size)) == 0)
    {
        // TODO: check for getrlimit(RLIMIT_DATA) ?????? dlmalloc doesn't call it
        if ((newsize = _allocate_mem(mem, size)) == 0)
        {
            errno = ENOMEM;
            return (0);
        }
    }

    if (!_chunk_is_splittable(_chunk_size_get(mem->chunk), size - FTMALLOC_MEM_CHUNK_SZ))
    {
        FTMALLOC_ASSERT(!_chunk_in_use_get(mem->chunk));

        if (mem->bin->head == mem->chunk)
        {
            FTMALLOC_ASSERT(_chunk_freed_prev_get(mem->bin->head) == NULL);
            mem->bin->head = _chunk_freed_next_get(mem->bin->head);
        }

        if (mem->chunk->freed_prev != NULL)
            _chunk_freed_next_set(mem->chunk->freed_prev, mem->chunk->freed_next);

        if (mem->chunk->freed_next != NULL)
            _chunk_freed_prev_set(mem->chunk->freed_next, mem->chunk->freed_prev);

        FTMALLOC_DEBUG_ONLY(_chunk_freed_prev_set(mem->chunk, NULL));
        FTMALLOC_DEBUG_ONLY(_chunk_freed_next_set(mem->chunk, NULL));
    }

    return (newsize);
}
