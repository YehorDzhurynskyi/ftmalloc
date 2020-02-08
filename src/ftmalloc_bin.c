/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftmalloc_bin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydzhuryn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/15 15:08:24 by ydzhuryn          #+#    #+#             */
/*   Updated: 2018/01/05 17:20:48 by ydzhuryn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftmalloc_internal.h"

void _bin_verify(const t_mem_bin* bin)
{
#ifdef FTMALLOC_DEBUG
    FTMALLOC_ASSERT(bin);
    FTMALLOC_ASSERT(FTMALLOC_MEM_ALIGNED_OK(bin->mem_allocated));
    FTMALLOC_ASSERT(FTMALLOC_MEM_ALIGNED_OK(bin->mem_occupied));
    FTMALLOC_ASSERT(FTMALLOC_MEM_ALIGNED_OK(bin->mem_user));
    FTMALLOC_ASSERT(bin->mem_occupied <= bin->mem_allocated);
    FTMALLOC_ASSERT(bin->mem_user < bin->mem_occupied);
    FTMALLOC_ASSERT(FTMALLOC_MEM_ALIGNED_OK(bin->next));

    if (bin->head != NULL)
    {
        FTMALLOC_ASSERT(FTMALLOC_MEM_ALIGNED_OK(bin->head));
        FTMALLOC_ASSERT(!_chunk_in_use_get(bin->head));
        FTMALLOC_ASSERT(_chunk_freed_prev_get(bin->head) == NULL);

        _chunk_verify(bin->head);
    }
#else
    (void)bin;
#endif
}

t_byte	*_bin_bin2mem(t_mem_bin* bin)
{
    _bin_verify(bin);
    return ((t_byte*)bin->head - FTMALLOC_MEM_CHUNK_SZ);
}

t_bool  _bin_is_empty(const t_mem_bin *bin)
{
    return (bin->mem_user == 0);
}

size_t      _bin_max_size_of(size_t size)
{
    if (size <= FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_SMALL)
    {
        return (FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_SMALL);
    }
    else if (size <= FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_MEDIUM)
    {
        return (FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_MEDIUM);
    }
    return (-1);
}

t_mem_bin	*_bin_list_of(size_t size)
{
	FTMALLOC_ASSERT(FTMALLOC_MEM_CHUNK_SZ_OK(size));

	if (size <= FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_SMALL)
	{
		return (g_ftmalloc_state.bin_list_small);
	}
	else if (size <= FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_MEDIUM)
	{
		return (g_ftmalloc_state.bin_list_medium);
	}
	return (g_ftmalloc_state.bin_list_large);
}

size_t		_bin_alloc_size_of(size_t size)
{
    FTMALLOC_ASSERT(FTMALLOC_MEM_CHUNK_SZ_OK(size));

    const size_t multiple = getpagesize();

	size_t alloc_size = 0;
	if (size <= FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_SMALL)
	{
		alloc_size = FTMALLOC_BIN_APPROX_MIN_NITEMS * FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_SMALL;
	}
	else if (size <= FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_MEDIUM)
	{
		alloc_size = FTMALLOC_BIN_APPROX_MIN_NITEMS * FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_MEDIUM;
	}
	else
	{
		alloc_size = ft_ceil((size + FTMALLOC_MEM_BIN_SZ) / (float)multiple) * multiple;
	}

    alloc_size = FTMALLOC_ALIGN_UP(alloc_size, multiple);

    FTMALLOC_ASSERT(FTMALLOC_MEM_CHUNK_SZ_OK(alloc_size));
	FTMALLOC_ASSERT(alloc_size % multiple == 0);
	return alloc_size;
}

t_mem_chunk *_bin_adj(const t_mem_bin *bin)
{
    t_mem_chunk *bottom_chunk;

    _bin_verify(bin);
    bottom_chunk = (t_mem_chunk*)((t_byte*)bin - FTMALLOC_MEM_CHUNK_SZ);
    return ((t_mem_chunk*)((t_byte*)bottom_chunk - bottom_chunk->prev_size - FTMALLOC_MEM_CHUNK_SZ));
}
