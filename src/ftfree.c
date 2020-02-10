/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftfree.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydzhuryn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/15 15:08:24 by ydzhuryn          #+#    #+#             */
/*   Updated: 2018/01/05 17:20:48 by ydzhuryn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftmalloc_internal.h"

static t_bool	try_bind_with_adj_chunks(t_mem *mem)
{
	t_mem_chunk	*merged_prev;
	t_mem_chunk	*merged_next;

	FTMALLOC_ASSERT(!chunk_in_use_get(mem->chunk));
	merged_prev = buddy_try_merge_prev(mem);
	FTMALLOC_ASSERT(!chunk_in_use_get(mem->chunk));
	if (merged_prev)
		mem->bin->mem_occupied -= FTMALLOC_MEM_CHUNK_SZ;
	FTMALLOC_ASSERT(!chunk_in_use_get(mem->chunk));
	merged_next = buddy_try_merge_next(mem);
	FTMALLOC_ASSERT(!chunk_in_use_get(mem->chunk));
	if (merged_next)
		mem->bin->mem_occupied -= FTMALLOC_MEM_CHUNK_SZ;
	if (merged_next != NULL)
	{
		FTMALLOC_ASSERT(merged_prev == NULL || (mem->chunk->freed_next ==
		merged_next && merged_next->freed_prev == mem->chunk));
		chunk_freed_next_set(mem->chunk, merged_next->freed_next);
		if (merged_prev == NULL)
			chunk_freed_prev_set(mem->chunk, merged_next->freed_prev);
		if (mem->bin->head > mem->chunk)
			mem->bin->head = mem->chunk;
	}
	return (merged_prev || merged_next);
}

static void		release(t_mem *mem, const size_t size, t_mem_bin *prev_bin)
{
	FTMALLOC_ASSERT(mem->bin->mem_user >= size);
	FTMALLOC_ASSERT(mem->bin->mem_occupied > size);
	mem->bin->mem_user -= size;
	mem->bin->mem_occupied -= size;
	bin_verify(mem->bin);
	if (!try_bind_with_adj_chunks(mem))
	{
		buddy_deoccupy(mem);
	}
	chunk_verify(mem->chunk);
	bin_verify(mem->bin);
	if (bin_is_empty(mem->bin))
	{
		mem_deallocate(mem, size, prev_bin);
	}
	else
	{
		ft_memset((t_byte*)chunk_chunk2mem(mem->chunk) + FTMALLOC_MEM_MIN_PAYLOAD_SZ, 0xfd, chunk_size_get(mem->chunk) - FTMALLOC_MEM_MIN_PAYLOAD_SZ);
	}
}

void			ftfree_internal(void *raw)
{
	t_mem_bin	*prev_bin;
	t_mem		mem;
	size_t		size;

	if (raw == NULL)
		return ;
	mem.chunk = chunk_mem2chunk(raw);
	FTMALLOC_ASSERT(FTMALLOC_MEM_ALIGNED_OK(raw));
	FTMALLOC_ASSERT(FTMALLOC_MEM_ALIGNED_OK(mem.chunk));
	FTMALLOC_ASSERT(chunk_in_use_get(mem.chunk));
	chunk_verify(mem.chunk);
	mem.bin = chunk_bin_of(mem.chunk, &prev_bin);
	if (mem.bin == NULL)
		return ;
	bin_verify(mem.bin);
	FTMALLOC_ASSERT(mem.bin == chunk_bin_of_slow(mem.chunk));
	chunk_in_use_set_false(mem.chunk);
	chunk_freed_next_set(mem.chunk, NULL);
	chunk_freed_prev_set(mem.chunk, NULL);
	size = chunk_size_get(mem.chunk);
	FTMALLOC_DEBUG_ONLY(g_ftmalloc_state.usage_dealloc++);
	FTMALLOC_DEBUG_ONLY(g_ftmalloc_state.total_dealloc += size);
	release(&mem, size, prev_bin);
}

void			ftfree(void *mem)
{
	if (FTMALLOC_LOCK != 0)
	{
		return ;
	}
	ftfree_internal(mem);
    FTMALLOC_DEBUG_ONLY(ftmalloc_check_heap_fully());
	FTMALLOC_UNLOCK;
}
