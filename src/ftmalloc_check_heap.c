/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftmalloc_check_heap.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydzhuryn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/15 15:08:24 by ydzhuryn          #+#    #+#             */
/*   Updated: 2018/01/05 17:20:48 by ydzhuryn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftmalloc_internal.h"

static t_bool	bin_verify_freed_links_internal(t_mem_bin *bin)
{
	t_mem_chunk	*prev;
	t_mem_chunk	*chunk;

	while (bin)
	{
		prev = NULL;
		chunk = bin->head;
		while (chunk)
		{
            if (chunk_in_use_get(chunk) ||
                prev != chunk->freed_prev ||
                chunk == chunk->freed_next)
            {
                return (FALSE);
            }
			prev = chunk;
			chunk = chunk->freed_next;
		}
		bin = bin->next;
	}
    return (TRUE);
}

int		    ftmalloc_check_heap_relaxed(void)
{
    t_bool is_valid;

    is_valid = TRUE;
    is_valid = is_valid && bin_verify_freed_links_internal(g_ftmalloc_state.bin_list_small);
    is_valid = is_valid && bin_verify_freed_links_internal(g_ftmalloc_state.bin_list_medium);
    is_valid = is_valid && bin_verify_freed_links_internal(g_ftmalloc_state.bin_list_large);

    // TODO: stderr print
    FTMALLOC_ASSERT(is_valid && "ftmalloc heap corruption error: invalid freed links!");

    return (is_valid);
}

static t_bool   chunk_verify_internal(t_mem_chunk *chunk, size_t bin_allocated, size_t min_chunk_sz, size_t max_chunk_sz)
{
    t_bool  is_valid;
    size_t  size;

    is_valid = TRUE;
    if (chunk_is_prev_top(chunk))
    {
        is_valid = is_valid && chunk->prev_size == 0;
    }
    else if (chunk_prev_in_use_get(chunk))
    {
        is_valid = is_valid && chunk->prev_size >= min_chunk_sz && chunk->prev_size <= max_chunk_sz;
    }
    else
    {
        is_valid = is_valid && chunk->prev_size < bin_allocated;
    }

    size = chunk_size_get(chunk);
    if (chunk_in_use_get(chunk))
    {
        is_valid = is_valid && size >= min_chunk_sz && size <= max_chunk_sz;
    }
    else
    {
        is_valid = is_valid && size < bin_allocated;
    }

    return (is_valid);
}

static t_bool   bin_verify_contiguous_internal(t_mem_bin *bin)
{
    t_mem_chunk	*prev;
    t_mem_chunk	*chunk;
    size_t       max_chunk_sz;
    size_t       min_chunk_sz;

    while (bin)
    {
        prev = NULL;

        if (bin->mem_allocated == FTMALLOC_BIN_APPROX_MIN_NITEMS * FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_SMALL)
        {
            max_chunk_sz = FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_SMALL - FTMALLOC_MEM_CHUNK_SZ;
            min_chunk_sz = FTMALLOC_MEM_MIN_PAYLOAD_SZ;
        }
        else if (bin->mem_allocated == FTMALLOC_BIN_APPROX_MIN_NITEMS * FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_MEDIUM)
        {
            max_chunk_sz = FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_MEDIUM - FTMALLOC_MEM_CHUNK_SZ;
            min_chunk_sz = FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_SMALL;
        }
        else
        {
            if (bin->mem_allocated % getpagesize() != 0)
            {
                return (FALSE);
            }
            max_chunk_sz = -1;
            min_chunk_sz = FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_MEDIUM;
        }
        chunk = bin_adj(bin);
        t_mem_chunk* bottom = chunk_adj_next(chunk);
        if (!chunk_is_next_bottom(chunk) || chunk_size_get(bottom) != 0 || bottom->prev_size != chunk_size_get(chunk))
        {
            return (FALSE);
        }
        while (!chunk_is_prev_top(chunk))
        {
            if (!chunk_verify_internal(chunk, bin->mem_allocated, min_chunk_sz, max_chunk_sz))
            {
                return (FALSE);
            }
            chunk = chunk_adj_prev(chunk);
        }
        if (!chunk_verify_internal(chunk, bin->mem_allocated, min_chunk_sz, max_chunk_sz))
        {
            return (FALSE);
        }
        bin = bin->next;
    }
    return (TRUE);
}

int		    ftmalloc_check_heap_fully(void)
{
    t_bool  is_valid;

    if (!ftmalloc_check_heap_relaxed())
    {
        return (FALSE);
    }

    is_valid = TRUE;
    is_valid = is_valid && bin_verify_contiguous_internal(g_ftmalloc_state.bin_list_small);
    is_valid = is_valid && bin_verify_contiguous_internal(g_ftmalloc_state.bin_list_medium);
    is_valid = is_valid && bin_verify_contiguous_internal(g_ftmalloc_state.bin_list_large);

    // TODO: stderr print
    FTMALLOC_ASSERT(is_valid && "ftmalloc heap corruption error: incorrect size in adjusted chunks!");

    return (is_valid);
}
