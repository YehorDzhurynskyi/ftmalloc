/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftmalloc_buddy.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydzhuryn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/15 15:08:24 by ydzhuryn          #+#    #+#             */
/*   Updated: 2018/01/05 17:20:48 by ydzhuryn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftmalloc_internal.h"

static void _chunk_split(t_mem *mem, const size_t size)
{
    t_mem_chunk *next;

    _chunk_verify(mem->chunk);
    FTMALLOC_ASSERT(FTMALLOC_MEM_CHUNK_SZ_OK(size));
    FTMALLOC_ASSERT(_chunk_size_get(mem->chunk) >= size + FTMALLOC_MEM_CHUNK_SZ);

    size_t osize = _chunk_size_get(mem->chunk);

    mem->chunk = _chunk_arrange(mem->chunk, size - FTMALLOC_MEM_CHUNK_SZ, _chunk_in_use_get(mem->chunk));
    next = _chunk_arrange(_chunk_adj_next(mem->chunk), osize - size, FALSE);

    FTMALLOC_ASSERT(!_chunk_in_use_get(next));

    if (!_chunk_in_use_get(mem->chunk))
    {
        _chunk_freed_next_set(next, mem->chunk->freed_next);
        _chunk_freed_prev_set(next, mem->chunk->freed_prev);
    }
    else
    {
        _chunk_freed_next_set(next, NULL);
        _chunk_freed_prev_set(next, NULL);
    }

    if (mem->bin->head == mem->chunk)
    {
        FTMALLOC_ASSERT(_chunk_freed_prev_get(mem->bin->head) == NULL);
        mem->bin->head = next;
    }

    mem->bin->mem_occupied += FTMALLOC_MEM_CHUNK_SZ;

    _chunk_verify(next);
    FTMALLOC_ASSERT(_chunk_adj_next(mem->chunk) == next);
    FTMALLOC_ASSERT(_chunk_adj_prev(next) == mem->chunk);
}

void _chunk_occupy(t_mem *mem, const size_t size)
{
    _chunk_verify(mem->chunk);
    FTMALLOC_ASSERT(FTMALLOC_MEM_CHUNK_SZ_OK(size));
    FTMALLOC_ASSERT(_chunk_size_get(mem->chunk) >= size - FTMALLOC_MEM_CHUNK_SZ);

    if (_chunk_is_splittable(_chunk_size_get(mem->chunk), size - FTMALLOC_MEM_CHUNK_SZ))
    {
        _chunk_split(mem, size);
    }
    else
    {
        FTMALLOC_ASSERT(
            _chunk_size_get(mem->chunk) - (size - FTMALLOC_MEM_CHUNK_SZ) == 0 ||
            _chunk_size_get(mem->chunk) - (size - FTMALLOC_MEM_CHUNK_SZ) == FTMALLOC_MEM_MIN_PAYLOAD_SZ
        );
        FTMALLOC_ASSERT(_chunk_size_get(mem->chunk) + FTMALLOC_MEM_CHUNK_SZ <= _bin_max_size_of(size));
    }

    _chunk_in_use_set_true(mem->chunk);

    _chunk_verify(mem->chunk);
}

void _chunk_deoccupy(t_mem* mem)
{
    if (mem->bin->head == NULL)
    {
        _chunk_freed_prev_set(mem->chunk, NULL);
        _chunk_freed_next_set(mem->chunk, NULL);
        mem->bin->head = mem->chunk;
    }
    else if (mem->bin->head > mem->chunk)
    {
        _chunk_freed_prev_set(mem->chunk, mem->bin->head->freed_prev);
        _chunk_freed_next_set(mem->chunk, mem->bin->head);

        FTMALLOC_ASSERT(mem->chunk->freed_prev == NULL);
        FTMALLOC_ASSERT(mem->chunk->freed_next != NULL);

        mem->bin->head = mem->chunk;
    }
    else
    {
        t_mem_chunk* freed = mem->bin->head;
        t_mem_chunk* next;
        while ((next = _chunk_freed_next_get(freed)) != NULL)
        {
            if (next > mem->chunk)
            {
                break;
            }
            freed = next;
        }

        if (next == NULL)
        {
            FTMALLOC_ASSERT(freed->freed_next == NULL);
            _chunk_freed_prev_set(mem->chunk, freed);
            _chunk_freed_next_set(mem->chunk, NULL);
        }
        else
        {
            FTMALLOC_ASSERT(next->freed_prev != NULL);
            _chunk_freed_prev_set(mem->chunk, next->freed_prev);
            _chunk_freed_next_set(mem->chunk, next);
        }
    }
}

static void	_chunk_merge(t_mem_chunk *from, t_mem_chunk *to)
{
	_chunk_verify(from);
	_chunk_verify(to);
	FTMALLOC_ASSERT(to == _chunk_adj_prev(from));
	FTMALLOC_ASSERT(from == _chunk_adj_next(to));

    const t_bool inuse = _chunk_in_use_get(to);
	_chunk_size_set(to, _chunk_size_get(to) + _chunk_size_get(from) + FTMALLOC_MEM_CHUNK_SZ);

    if (inuse)
    {
        _chunk_in_use_set_true(to);
    }
    else
    {
        _chunk_in_use_set_false(to);
    }

	_chunk_verify(to);
}

t_mem_chunk *_chunk_try_merge_next(t_mem *mem)
{
    t_mem_chunk *merged;

    _chunk_verify(mem->chunk);

    merged = NULL;
    if (!_chunk_is_next_bottom(mem->chunk))
    {
        merged = _chunk_adj_next(mem->chunk);
        if (_chunk_in_use_get(merged))
        {
            return (NULL);
        }
        _chunk_merge(merged, mem->chunk);
    }

    _chunk_verify(mem->chunk);
    return (merged);
}

t_mem_chunk	*_chunk_try_merge_prev(t_mem *mem)
{
    t_mem_chunk *merged;
    t_mem_chunk *prev;

    _chunk_verify(mem->chunk);

    merged = NULL;
    if (!_chunk_is_prev_top(mem->chunk) && !_chunk_prev_in_use_get(mem->chunk))
    {
        merged = mem->chunk;
        prev = _chunk_adj_prev(mem->chunk);
        _chunk_merge(mem->chunk, prev);
        mem->chunk = prev;
    }

    _chunk_verify(mem->chunk);

    return (merged);
}
