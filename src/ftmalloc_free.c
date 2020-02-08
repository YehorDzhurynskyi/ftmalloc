/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftmalloc_free.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydzhuryn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/15 15:08:24 by ydzhuryn          #+#    #+#             */
/*   Updated: 2018/01/05 17:20:48 by ydzhuryn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftmalloc_internal.h"

void ftfree_internal(void *raw)
{
    t_mem       mem;
    t_mem_bin   *prev_bin;
    size_t      size;

    if (!raw)
    {
        return;
    }

    mem.chunk = _chunk_mem2chunk(raw);

    FTMALLOC_ASSERT(FTMALLOC_MEM_ALIGNED_OK(raw));
    FTMALLOC_ASSERT(FTMALLOC_MEM_ALIGNED_OK(mem.chunk));
    FTMALLOC_ASSERT(_chunk_in_use_get(mem.chunk));
    _chunk_verify(mem.chunk);

    mem.bin = _chunk_bin_of(mem.chunk, &prev_bin);
    if (!mem.bin)
    {
        return;
    }

    _bin_verify(mem.bin);
    FTMALLOC_ASSERT(mem.bin == _chunk_bin_of_slow(mem.chunk));

    _chunk_in_use_set_false(mem.chunk);
    FTMALLOC_DEBUG_ONLY(_chunk_freed_next_set(mem.chunk, NULL));
    FTMALLOC_DEBUG_ONLY(_chunk_freed_prev_set(mem.chunk, NULL));
    size = _chunk_size_get(mem.chunk);

#ifdef FTMALLOC_DEBUG
    g_ftmalloc_state.usage_dealloc++;
    g_ftmalloc_state.total_dealloc += size;
#endif

    FTMALLOC_ASSERT(mem.bin->mem_user >= size);
    FTMALLOC_ASSERT(mem.bin->mem_occupied > size);

    mem.bin->mem_user -= size;
    mem.bin->mem_occupied -= size;

    _bin_verify(mem.bin);

    FTMALLOC_ASSERT(!_chunk_in_use_get(mem.chunk));
    t_mem_chunk *merged_prev = _chunk_try_merge_prev(&mem);
    FTMALLOC_ASSERT(!_chunk_in_use_get(mem.chunk));
    if (merged_prev) mem.bin->mem_occupied -= FTMALLOC_MEM_CHUNK_SZ;

    FTMALLOC_ASSERT(!_chunk_in_use_get(mem.chunk));
    t_mem_chunk *merged_next = _chunk_try_merge_next(&mem);
    FTMALLOC_ASSERT(!_chunk_in_use_get(mem.chunk));
    if (merged_next) mem.bin->mem_occupied -= FTMALLOC_MEM_CHUNK_SZ;

    if (merged_next == NULL && merged_prev == NULL)
    {
        _chunk_deoccupy(&mem);
    }
    else if (merged_next != NULL)
    {
        FTMALLOC_ASSERT(merged_prev == NULL || (mem.chunk->freed_next == merged_next && merged_next->freed_prev == mem.chunk));

        _chunk_freed_next_set(mem.chunk, merged_next->freed_next);

        if (merged_prev == NULL)
            _chunk_freed_prev_set(mem.chunk, merged_next->freed_prev);

        if (mem.bin->head > mem.chunk)
            mem.bin->head = mem.chunk;
    }

    _chunk_verify(mem.chunk);
    _bin_verify(mem.bin);

    if (_bin_is_empty(mem.bin))
    {
        if (prev_bin == NULL)
        {
            if (size + FTMALLOC_MEM_CHUNK_SZ <= FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_SMALL)
            {
                g_ftmalloc_state.bin_list_small = mem.bin->next;
            }
            else if (size + FTMALLOC_MEM_CHUNK_SZ <= FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_MEDIUM)
            {
                g_ftmalloc_state.bin_list_medium = mem.bin->next;
            }
            else
            {
                g_ftmalloc_state.bin_list_large = mem.bin->next;
            }
        }
        else
        {
            prev_bin->next = mem.bin->next;
        }

        if (munmap(_bin_bin2mem(mem.bin), mem.bin->mem_allocated) == -1)
        {
            FTMALLOC_ASSERT(!"Unexpected fail or invalid param");
        }

        FTMALLOC_DEBUG_ONLY(g_ftmalloc_state.total_munmap++);
    }
    // TODO: check if it is needed to be defragmented
    // TODO: set errno?

}

void    ftfree(void *mem)
{
    if (FTMALLOC_LOCK != 0)
    {
        return;
    }

    ftfree_internal(mem);

    _ftmalloc_verify_freed_links();

    FTMALLOC_UNLOCK;
}
