/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftmalloc_realloc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydzhuryn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/15 15:08:24 by ydzhuryn          #+#    #+#             */
/*   Updated: 2018/01/05 17:20:48 by ydzhuryn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftmalloc_internal.h"

static t_mem_chunk     *_rebind_freed_links(t_mem *mem, t_mem_chunk *freed_prev, t_mem_chunk *freed_next)
{
    t_mem_chunk *local_head = NULL;
    t_mem_chunk *adjnext = _chunk_adj_next(mem->chunk);
    if (_chunk_in_use_get(adjnext))
    {
        if (freed_prev) _chunk_freed_next_set(freed_prev, freed_next);
        if (freed_next) _chunk_freed_prev_set(freed_next, freed_prev);

        local_head = freed_prev ? freed_prev : freed_next;
    }
    else
    {
        if (!_chunk_is_next_bottom(mem->chunk))
        {
            _chunk_freed_prev_set(adjnext, freed_prev);
            if (adjnext != freed_next)
            {
                _chunk_freed_next_set(adjnext, freed_next);
            }

            local_head = adjnext;
        }
        else
        {
            if (freed_prev) _chunk_freed_next_set(freed_prev, NULL);

            local_head = NULL;
        }
    }

    return (local_head);
}

static t_bool	ftrealloc_try_satisfy_size(t_byte *raw, size_t size)
{
    t_mem   mem;
    size_t  oldsize;
    size_t  adjsize;

    mem.chunk = _chunk_mem2chunk(raw);
    mem.bin = _chunk_bin_of(mem.chunk, NULL);

    _bin_verify(mem.bin);
    _chunk_verify(mem.chunk);
    FTMALLOC_ASSERT(mem.bin == _chunk_bin_of_slow(mem.chunk));
    FTMALLOC_ASSERT(_chunk_in_use_get(mem.chunk));

    oldsize = _chunk_size_get(mem.chunk);

    size = FTMALLOC_MEM_ALIGN_UP(size);
    if (size == oldsize)
    {
        return (TRUE);
    }
    else if (size > oldsize)
    {
        if (size + FTMALLOC_MEM_CHUNK_SZ > _bin_max_size_of(oldsize))
        {
            return (FALSE);
        }

        if (!_chunk_is_next_bottom(mem.chunk))
        {
            t_mem_chunk *next = _chunk_adj_next(mem.chunk);
            if (!_chunk_in_use_get(next))
            {
                adjsize = _chunk_size_get(next);
                if (oldsize + FTMALLOC_MEM_CHUNK_SZ + adjsize >= size &&
                    oldsize + 2 * FTMALLOC_MEM_CHUNK_SZ + adjsize <= _bin_max_size_of(size))
                {
                    t_mem_chunk *merged_next = _chunk_try_merge_next(&mem);
                    FTMALLOC_ASSERT(merged_next != NULL);
                    if (merged_next) mem.bin->mem_occupied -= FTMALLOC_MEM_CHUNK_SZ;

                    t_mem_chunk *merged_next_freed_prev = merged_next->freed_prev;
                    t_mem_chunk *merged_next_freed_next = merged_next->freed_next;

                    FTMALLOC_ASSERT(_chunk_size_get(mem.chunk) == oldsize + FTMALLOC_MEM_CHUNK_SZ + adjsize);
                    FTMALLOC_ASSERT(_chunk_size_get(mem.chunk) >= size);
                    FTMALLOC_ASSERT(_chunk_in_use_get(mem.chunk));

                    _chunk_occupy(&mem, size + FTMALLOC_MEM_CHUNK_SZ);

                    const t_mem_chunk *local_head = _rebind_freed_links(&mem, merged_next_freed_prev, merged_next_freed_next);
                    if (mem.bin->head == merged_next)
                    {
                        mem.bin->head = local_head;
                    }

                    FTMALLOC_ASSERT(_chunk_size_get(mem.chunk) > oldsize);
                    size_t dsize = _chunk_size_get(mem.chunk) - oldsize;

                    mem.bin->mem_user += dsize;
                    mem.bin->mem_occupied += dsize;

                    FTMALLOC_DEBUG_ONLY(g_ftmalloc_state.total_alloc += dsize);

                    _bin_verify(mem.bin);
                    _chunk_verify(mem.chunk);
                    FTMALLOC_ASSERT(mem.bin == _chunk_bin_of(mem.chunk, NULL) && mem.bin == _chunk_bin_of_slow(mem.chunk));

                    return (TRUE);
                }
            }
        }
    }
    else if (size < oldsize)
    {
        if (oldsize + FTMALLOC_MEM_CHUNK_SZ > _bin_max_size_of(size))
        {
            return (FALSE);
        }

        if (!_chunk_is_splittable(_chunk_size_get(mem.chunk), size))
        {
            return (TRUE);
        }

        _chunk_occupy(&mem, size + FTMALLOC_MEM_CHUNK_SZ);

        t_mem adjmem;

        adjmem.bin = mem.bin;
        adjmem.chunk = _chunk_adj_next(mem.chunk);

        _chunk_deoccupy(&adjmem);

        FTMALLOC_ASSERT(oldsize >= _chunk_size_get(mem.chunk));
        size_t dsize = oldsize - _chunk_size_get(mem.chunk);

        mem.bin->mem_user -= dsize;
        mem.bin->mem_occupied -= dsize;

        FTMALLOC_DEBUG_ONLY(g_ftmalloc_state.total_dealloc += dsize);

        _bin_verify(mem.bin);
        _chunk_verify(mem.chunk);

        FTMALLOC_ASSERT(mem.bin == _chunk_bin_of(mem.chunk, NULL) && mem.bin == _chunk_bin_of_slow(mem.chunk));

        return (TRUE);
    }
    else
    {
        FTMALLOC_ASSERT(!"Unhandled case");
    }

	return (FALSE);
}

void	*ftrealloc_internal(void *oldmem, size_t size)
{
	void *mem;

	if (oldmem == NULL)
	{
		return (ftmalloc_internal(size));
	}

	if (size == 0)
	{
        ftfree_internal(oldmem);
        return (NULL);
	}

	if (ftrealloc_try_satisfy_size(oldmem, size))
	{
		mem = oldmem;
        FTMALLOC_DEBUG_ONLY(g_ftmalloc_state.total_realloc_hits++);
	}
	else
	{
		mem = ftmalloc_internal(size);
		if (mem != NULL)
		{
            t_mem_chunk* oldchunk = _chunk_mem2chunk(oldmem);
            size_t oldsize = _chunk_size_get(oldchunk);
            size_t copiedsize = size < oldsize ? size : oldsize;
            ft_memcpy(mem, oldmem, copiedsize);
            FTMALLOC_DEBUG_ONLY(g_ftmalloc_state.total_alloc_copied += copiedsize);
			ftfree_internal(oldmem);
		}
	}
    FTMALLOC_DEBUG_ONLY(g_ftmalloc_state.usage_realloc += mem ? 1 : 0);
	return (mem);
}

// TODO: realloc works bad:
// `bin->head` is moving in opposite way:
// =====================================
//    Example:
//      void *a = ftmalloc(1);
//      a = ftrealloc(a, 17);
// =====================================
// so after bucket mmap'ing we have a lot of freed memory
// but realloc will cause a new mmap'ing because realloc
// looking for next adjusted chunk which is bottom in this example
//    Solution:
//      reverse memory growth direction in opposite way
void	*ftrealloc(void *oldmem, size_t size)
{
	// TODO: test edge cases
    void *mem;

    if (FTMALLOC_LOCK != 0)
    {
        return (NULL);
    }

    mem = ftrealloc_internal(oldmem, size);

    _ftmalloc_verify_freed_links();

    FTMALLOC_UNLOCK;

    return (mem);
}
