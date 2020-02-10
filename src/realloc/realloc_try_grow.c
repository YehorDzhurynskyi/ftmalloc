/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc_grow.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydzhuryn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/15 15:08:24 by ydzhuryn          #+#    #+#             */
/*   Updated: 2018/01/05 17:20:48 by ydzhuryn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftmalloc_internal.h"

static t_mem_chunk	*bind_with_adj(t_mem *mem,
t_mem_chunk *freed_prev,
t_mem_chunk *freed_next,
t_mem_chunk *adjnext)
{
	t_mem_chunk	*local_head;

	if (!chunk_is_next_bottom(mem->chunk))
	{
		chunk_freed_prev_set(adjnext, freed_prev);
		if (adjnext != freed_next)
		{
			chunk_freed_next_set(adjnext, freed_next);
		}
		local_head = adjnext;
	}
	else
	{
		if (freed_prev)
			chunk_freed_next_set(freed_prev, NULL);
		local_head = NULL;
	}
	return (local_head);
}

static t_mem_chunk	*rebind_freed_links(t_mem *mem,
t_mem_chunk *freed_prev,
t_mem_chunk *freed_next)
{
	t_mem_chunk	*local_head;
	t_mem_chunk	*adjnext;

	local_head = NULL;
	adjnext = chunk_adj_next(mem->chunk);
	if (chunk_in_use_get(adjnext))
	{
		if (freed_prev)
			chunk_freed_next_set(freed_prev, freed_next);
		if (freed_next)
			chunk_freed_prev_set(freed_next, freed_prev);
		local_head = freed_prev ? freed_prev : freed_next;
	}
	else
	{
		local_head = bind_with_adj(mem, freed_prev, freed_next, adjnext);
	}
	return (local_head);
}

static void			do_grow(t_mem *mem, t_mem_chunk *merged, size_t size)
{
	t_mem_chunk	*merged_freed_prev;
	t_mem_chunk	*merged_freed_next;
	t_mem_chunk	*local_head;

	merged_freed_prev = merged->freed_prev;
	merged_freed_next = merged->freed_next;
	FTMALLOC_ASSERT(chunk_size_get(mem->chunk) >= size);
	FTMALLOC_ASSERT(chunk_in_use_get(mem->chunk));
	buddy_occupy(mem, size + FTMALLOC_MEM_CHUNK_SZ);
	local_head = rebind_freed_links(mem, merged_freed_prev, merged_freed_next);
	if (mem->bin->head == merged)
	{
		mem->bin->head = local_head;
	}
}

static void			realloc_grow(t_mem *mem,
size_t size,
size_t osize,
size_t adjsize)
{
	t_mem_chunk	*merged_next;
	size_t		dsize;

	merged_next = buddy_try_merge_next(mem);
	FTMALLOC_ASSERT(merged_next != NULL);
	if (merged_next)
		mem->bin->mem_occupied -= FTMALLOC_MEM_CHUNK_SZ;
	FTMALLOC_ASSERT(chunk_size_get(mem->chunk) ==
	osize + FTMALLOC_MEM_CHUNK_SZ + adjsize);
	do_grow(mem, merged_next, size);
	FTMALLOC_ASSERT(chunk_size_get(mem->chunk) > osize);
	dsize = chunk_size_get(mem->chunk) - osize;
	mem->bin->mem_user += dsize;
	mem->bin->mem_occupied += dsize;
    if (getenv(FTMALLOC_ENV_SCRIBBLE))
        ft_memset((t_byte*)chunk_chunk2mem(mem->chunk) + osize, 0xfa, dsize);
    FTMALLOC_DEBUG_ONLY(g_ftmalloc_state.total_alloc += dsize);
	bin_verify(mem->bin);
	chunk_verify(mem->chunk);
	FTMALLOC_ASSERT(mem->bin == chunk_bin_of(mem->chunk, NULL) &&
	mem->bin == chunk_bin_of_slow(mem->chunk));
}

t_bool				realloc_try_grow(t_mem *mem, size_t size, size_t osize)
{
	t_mem_chunk	*next;
	size_t		adjsize;

	if (size + FTMALLOC_MEM_CHUNK_SZ > bin_max_size_of(osize) ||
	chunk_is_next_bottom(mem->chunk))
	{
		return (FALSE);
	}
	next = chunk_adj_next(mem->chunk);
	if (!chunk_in_use_get(next))
	{
		adjsize = chunk_size_get(next);
		if (osize + FTMALLOC_MEM_CHUNK_SZ + adjsize >= size &&
		osize + 2 * FTMALLOC_MEM_CHUNK_SZ + adjsize <= bin_max_size_of(size))
		{
			realloc_grow(mem, size, osize, adjsize);
			return (TRUE);
		}
	}
	return (FALSE);
}
