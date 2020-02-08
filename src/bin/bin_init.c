/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bin_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydzhuryn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/15 15:08:24 by ydzhuryn          #+#    #+#             */
/*   Updated: 2018/01/05 17:20:48 by ydzhuryn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftmalloc_internal.h"

static t_mem_bin	*bin_arrange_header(t_byte *mem, size_t size)
{
	t_mem_bin	*bin;
	t_mem_chunk	*bottom_chunk;
	size_t		offset;
	size_t		payload;

	offset = size - FTMALLOC_MEM_ALIGN_UP(sizeof(t_mem_bin));
	payload = offset - 3 * FTMALLOC_MEM_CHUNK_SZ;
	bin = (t_mem_bin*)(mem + offset);
	bin->mem_user = 0;
	bin->mem_occupied = FTMALLOC_MEM_BIN_SZ + FTMALLOC_MEM_CHUNK_SZ;
	bin->mem_allocated = size;
	bin->next = NULL;
	bin->head = chunk_arrange(mem + FTMALLOC_MEM_CHUNK_SZ, payload, FALSE);
	chunk_freed_next_set(bin->head, NULL);
	chunk_freed_prev_set(bin->head, NULL);
	bottom_chunk = (t_mem_chunk*)((t_byte*)bin - FTMALLOC_MEM_CHUNK_SZ);
	bottom_chunk->prev_size = payload;
	bottom_chunk->size = 0;
	return (bin);
}

t_mem_bin			*bin_init_mem(t_byte *mem, size_t size)
{
	t_mem_bin	*bin;
	t_mem_chunk	*top_chunk;

	FTMALLOC_ASSERT(mem != NULL);
	FTMALLOC_ASSERT(FTMALLOC_MEM_ALIGNED_OK(mem));
	FTMALLOC_ASSERT(size % getpagesize() == 0);
	bin = bin_arrange_header(mem, size);
	top_chunk = (t_mem_chunk*)mem;
	top_chunk->size = 0;
	chunk_adj_next(top_chunk)->prev_size = 0;
	chunk_prev_in_use_set_true(bin->head);
	bin_verify(bin);
	chunk_verify(bin->head);
	FTMALLOC_ASSERT(FTMALLOC_MEM_ALIGNED_OK(top_chunk));
	FTMALLOC_ASSERT(bin->head == _bin_adj(bin));
	FTMALLOC_ASSERT(_chunk_adj_prev(bin->head) == top_chunk);
	FTMALLOC_ASSERT(_chunk_adj_next(bin->head) == bottom_chunk);
	FTMALLOC_ASSERT(_chunk_is_prev_top(bin->head));
	FTMALLOC_ASSERT(_chunk_is_next_bottom(bin->head));
	return (bin);
}
