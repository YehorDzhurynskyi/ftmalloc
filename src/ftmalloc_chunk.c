/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftmalloc_chunk.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydzhuryn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/15 15:08:24 by ydzhuryn          #+#    #+#             */
/*   Updated: 2018/01/05 17:20:48 by ydzhuryn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftmalloc_internal.h"

void	_chunk_verify(const t_mem_chunk* chunk)
{
#ifdef FTMALLOC_DEBUG
    FTMALLOC_ASSERT(chunk);
	FTMALLOC_ASSERT(FTMALLOC_MEM_ALIGNED_OK(chunk));
	FTMALLOC_ASSERT(FTMALLOC_MEM_CHUNK_SZ_OK(_chunk_size_get(chunk)));
	FTMALLOC_ASSERT(_chunk_size_get(chunk) == _chunk_adj_next(chunk)->prev_size);
    FTMALLOC_ASSERT(chunk->prev_size == _chunk_size_get(_chunk_adj_prev(chunk)));

    if (!_chunk_is_prev_top(chunk))
    {
        FTMALLOC_ASSERT(FTMALLOC_MEM_CHUNK_SZ_OK(chunk->prev_size));
    }

    if (!_chunk_in_use_get(chunk))
    {
        if (_chunk_freed_next_get(chunk) != NULL)
        {
            FTMALLOC_ASSERT(_chunk_freed_next_get(chunk)->freed_prev == chunk);
            FTMALLOC_ASSERT(!_chunk_in_use_get(_chunk_freed_next_get(chunk)));
        }

        if (_chunk_freed_prev_get(chunk) != NULL)
        {
            FTMALLOC_ASSERT(_chunk_freed_prev_get(chunk)->freed_next == chunk);
            FTMALLOC_ASSERT(!_chunk_in_use_get(_chunk_freed_prev_get(chunk)));
        }
    }
#else
	(void)chunk;
#endif
}

t_bool  _chunk_is_splittable(const size_t oldsize, const size_t newsize)
{
    FTMALLOC_ASSERT(oldsize >= newsize);
    return (oldsize - newsize > FTMALLOC_MEM_MIN_PAYLOAD_SZ);
}

t_bool	_chunk_is_prev_top(const t_mem_chunk *chunk)
{
    FTMALLOC_ASSERT(chunk != NULL);
    FTMALLOC_ASSERT(FTMALLOC_MEM_ALIGNED_OK(chunk));

    return (chunk->prev_size == 0 && _chunk_prev_in_use_get(chunk));
}

t_bool	_chunk_is_next_bottom(const t_mem_chunk* chunk)
{
    FTMALLOC_ASSERT(chunk != NULL);
    FTMALLOC_ASSERT(FTMALLOC_MEM_ALIGNED_OK(chunk));

    return (_chunk_size_get(_chunk_adj_next(chunk)) == 0);
}

t_byte	*_chunk_chunk2mem(t_mem_chunk *chunk)
{
    t_byte	*mem;

	FTMALLOC_ASSERT(chunk != NULL);
	mem = (t_byte*)chunk + FTMALLOC_MEM_CHUNK_SZ;
	FTMALLOC_ASSERT(FTMALLOC_MEM_ALIGNED_OK(mem));

	return (mem);
}

t_mem_chunk	*_chunk_mem2chunk(t_byte	*mem)
{
	t_mem_chunk	*chunk;

	FTMALLOC_ASSERT(mem != NULL);
	chunk = (t_mem_chunk*)((t_byte*)mem - FTMALLOC_MEM_CHUNK_SZ);
	FTMALLOC_ASSERT(FTMALLOC_MEM_ALIGNED_OK(chunk));

	return (chunk);
}

void _chunk_prev_in_use_set_true(t_mem_chunk* chunk)
{
	FTMALLOC_ASSERT(chunk != NULL);
	chunk->size |= FTMALLOC_PREV_IN_USE_BIT;
}

void _chunk_prev_in_use_set_false(t_mem_chunk* chunk)
{
	FTMALLOC_ASSERT(chunk != NULL);
	chunk->size &= ~FTMALLOC_PREV_IN_USE_BIT;
}

t_bool _chunk_prev_in_use_get(const t_mem_chunk* chunk)
{
	FTMALLOC_ASSERT(chunk != NULL);
	return (chunk->size & FTMALLOC_PREV_IN_USE_BIT);
}

void _chunk_in_use_set_true(t_mem_chunk* chunk)
{
	FTMALLOC_ASSERT(chunk != NULL);
	_chunk_prev_in_use_set_true(_chunk_adj_next(chunk));
}

void _chunk_in_use_set_false(t_mem_chunk* chunk)
{
	FTMALLOC_ASSERT(chunk != NULL);
	_chunk_prev_in_use_set_false(_chunk_adj_next(chunk));
}

t_bool _chunk_in_use_get(const t_mem_chunk* chunk)
{
	FTMALLOC_ASSERT(chunk != NULL);
	return (_chunk_prev_in_use_get(_chunk_adj_next(chunk)));
}

void _chunk_size_set(t_mem_chunk* chunk, size_t size)
{
	FTMALLOC_ASSERT(chunk != NULL);
	FTMALLOC_ASSERT(FTMALLOC_MEM_CHUNK_SZ_OK(size));
	chunk->size = (size & ~FTMALLOC_SERVICE_BITS) | (chunk->size & FTMALLOC_SERVICE_BITS);
	_chunk_adj_next(chunk)->prev_size = size;
}

size_t _chunk_size_get(const t_mem_chunk* chunk)
{
	size_t size;

	FTMALLOC_ASSERT(chunk != NULL);
	size = chunk->size & ~FTMALLOC_SERVICE_BITS;
	return (size);
}

t_mem_chunk* _chunk_adj_prev(const t_mem_chunk* chunk)
{
	FTMALLOC_ASSERT(chunk != NULL);
	return ((t_mem_chunk*)((t_byte*)chunk - FTMALLOC_MEM_CHUNK_SZ - chunk->prev_size));
}

t_mem_chunk* _chunk_adj_next(const t_mem_chunk* chunk)
{
	FTMALLOC_ASSERT(chunk != NULL);
	return ((t_mem_chunk*)((t_byte*)chunk + FTMALLOC_MEM_CHUNK_SZ + _chunk_size_get(chunk)));
}

static void _ftmalloc_verify_freed_links_internal(t_mem_bin *bin)
{
#ifdef FTMALLOC_DEBUG
    while (bin)
    {
        t_mem_chunk *prev = NULL;
        t_mem_chunk *chunk = bin->head;
        while (chunk)
        {
            FTMALLOC_ASSERT(!_chunk_in_use_get(chunk));
            FTMALLOC_ASSERT(prev == chunk->freed_prev);
            prev = chunk;

            FTMALLOC_ASSERT(chunk != chunk->freed_next);
            chunk = chunk->freed_next;
        }
        bin = bin->next;
    }
#endif
}

void _ftmalloc_verify_freed_links()
{
    _ftmalloc_verify_freed_links_internal(g_ftmalloc_state.bin_list_small);
    _ftmalloc_verify_freed_links_internal(g_ftmalloc_state.bin_list_medium);
    _ftmalloc_verify_freed_links_internal(g_ftmalloc_state.bin_list_large);
}

t_mem_chunk* _chunk_freed_prev_get(const t_mem_chunk* chunk)
{
	t_mem_chunk* freed_prev;

	FTMALLOC_ASSERT(chunk != NULL);
	FTMALLOC_ASSERT(!_chunk_in_use_get(chunk));
	freed_prev = chunk->freed_prev;
	FTMALLOC_ASSERT(!freed_prev || !_chunk_in_use_get(freed_prev));

	return (freed_prev);
}

t_mem_chunk* _chunk_freed_next_get(const t_mem_chunk* chunk)
{
	t_mem_chunk* freed_next;

	FTMALLOC_ASSERT(chunk != NULL);
	FTMALLOC_ASSERT(!_chunk_in_use_get(chunk));
	freed_next = chunk->freed_next;
	FTMALLOC_ASSERT(!freed_next || !_chunk_in_use_get(freed_next));

	return (freed_next);
}

void _chunk_freed_prev_set(t_mem_chunk *chunk, t_mem_chunk *freed)
{
	FTMALLOC_ASSERT(chunk != NULL);
    FTMALLOC_ASSERT(chunk != freed);
	FTMALLOC_ASSERT(!_chunk_in_use_get(chunk));
	FTMALLOC_ASSERT(!freed || !_chunk_in_use_get(freed));
	chunk->freed_prev = freed;
	if (freed) freed->freed_next = chunk;
}

void _chunk_freed_next_set(t_mem_chunk *chunk, t_mem_chunk *freed)
{
	FTMALLOC_ASSERT(chunk != NULL);
    FTMALLOC_ASSERT(chunk != freed);
	FTMALLOC_ASSERT(!_chunk_in_use_get(chunk));
	FTMALLOC_ASSERT(!freed || !_chunk_in_use_get(freed));
	chunk->freed_next = freed;
	if (freed) freed->freed_prev = chunk;
}

t_mem_bin   *_chunk_bin_of(const t_mem_chunk *chunk, t_mem_bin **prev_bin)
{
    t_mem_bin *bin;

    _chunk_verify(chunk);

    bin = _bin_list_of(_chunk_size_get(chunk) + FTMALLOC_MEM_CHUNK_SZ);
    if (prev_bin != NULL) *prev_bin = NULL;

    _bin_verify(bin);

    while (bin)
    {
        _bin_verify(bin);

        const t_bool in_range =
            chunk >= (t_mem_chunk*)((t_byte*)bin - bin->mem_allocated) &&
            chunk < (t_mem_chunk*)bin;

        if (in_range)
        {
            break;
        }
        if (prev_bin != NULL) *prev_bin = bin;
        bin = bin->next;
    }

    return (bin);
}

t_mem_bin   *_chunk_bin_of_slow(const t_mem_chunk *chunk)
{
    while (!_chunk_is_next_bottom(chunk))
    {
        chunk = _chunk_adj_next(chunk);
    }
    chunk = _chunk_adj_next(chunk);

    return ((t_mem_chunk*)((t_byte*)chunk + FTMALLOC_MEM_CHUNK_SZ));
}

t_mem_chunk	*_chunk_arrange(t_byte *mem, size_t size, t_bool inuse)
{
    t_mem_chunk* chunk;

    FTMALLOC_ASSERT(mem != NULL);
    FTMALLOC_ASSERT(FTMALLOC_MEM_ALIGNED_OK(mem));
    FTMALLOC_ASSERT(FTMALLOC_MEM_CHUNK_SZ_OK(size));

    chunk = (t_mem_chunk*)mem;
    _chunk_size_set(chunk, size);

    if (inuse)
    {
        _chunk_in_use_set_true(chunk);
    }
    else
    {
        _chunk_in_use_set_false(chunk);
    }

    return (chunk);
}
