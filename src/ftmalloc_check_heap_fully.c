/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftmalloc_check_heap_fully.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydzhuryn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/15 15:08:24 by ydzhuryn          #+#    #+#             */
/*   Updated: 2018/01/05 17:20:48 by ydzhuryn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftmalloc_internal.h"

#define ERR_MSG "ftmalloc heap corruption error: memory has been corrupted!"

static t_bool	chunk_verify_internal(t_mem_chunk *chunk,
size_t bin_allocated,
size_t min_chunk_sz,
size_t max_chunk_sz)
{
	t_bool	v;
	size_t	size;

	v = TRUE;
	if (chunk_is_prev_top(chunk))
	{
		v = v && chunk->prev_size == 0;
	}
	else if (chunk_prev_in_use_get(chunk))
	{
		v = v && chunk->prev_size >= min_chunk_sz &&
		chunk->prev_size <= max_chunk_sz;
	}
	else
	{
		v = v && chunk->prev_size < bin_allocated;
	}
	size = chunk_size_get(chunk);
	if (chunk_in_use_get(chunk))
		v = v && size >= min_chunk_sz && size <= max_chunk_sz;
	else
		v = v && size < bin_allocated;
	return (v);
}

static void		determine_limits(t_mem_bin *bin,
size_t *min_chunk_sz,
size_t *max_chunk_sz)
{
	if (bin->mem_allocated == FTMALLOC_BIN_APPROX_MIN_NITEMS *
	FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_SMALL)
	{
		*min_chunk_sz = FTMALLOC_MIN_SZ;
		*max_chunk_sz = FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_SMALL -
		FTMALLOC_CHUNK_SZ;
	}
	else if (bin->mem_allocated == FTMALLOC_BIN_APPROX_MIN_NITEMS *
	FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_MEDIUM)
	{
		*min_chunk_sz = FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_SMALL;
		*max_chunk_sz = FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_MEDIUM -
		FTMALLOC_CHUNK_SZ;
	}
	else
	{
		*min_chunk_sz = FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_MEDIUM;
		*max_chunk_sz = -1;
	}
}

static t_bool	verify_chunks(t_mem_bin *bin,
t_mem_chunk *chunk,
size_t min_chunk_sz,
size_t max_chunk_sz)
{
	while (!chunk_is_prev_top(chunk))
	{
		if (!chunk_verify_internal(chunk, bin->mem_allocated,
		min_chunk_sz, max_chunk_sz))
			return (FALSE);
		chunk = chunk_adj_prev(chunk);
	}
	if (!chunk_verify_internal(chunk, bin->mem_allocated,
	min_chunk_sz, max_chunk_sz))
		return (FALSE);
	return (TRUE);
}

static t_bool	bin_verify_contiguous_internal(t_mem_bin *bin)
{
	t_mem_chunk	*prev;
	t_mem_chunk	*chunk;
	t_mem_chunk *bottom;
	size_t		max_chunk_sz;
	size_t		min_chunk_sz;

	while (bin)
	{
		prev = NULL;
		if (bin->mem_allocated % getpagesize() != 0)
			return (FALSE);
		determine_limits(bin, &min_chunk_sz, &max_chunk_sz);
		chunk = bin_adj_bottom(bin);
		bottom = chunk_adj_next(chunk);
		if (!chunk_is_next_bottom(chunk) || chunk_size_get(bottom) != 0 ||
		bottom->prev_size != chunk_size_get(chunk))
			return (FALSE);
		if (!verify_chunks(bin, chunk, min_chunk_sz, max_chunk_sz))
			return (FALSE);
		bin = bin->next;
	}
	return (TRUE);
}

int				ftmalloc_check_heap_fully(void)
{
	t_bool	v;

	if (!ftmalloc_check_heap_relaxed())
	{
		return (FALSE);
	}
	v = TRUE;
	v = v && bin_verify_contiguous_internal(g_ftmalloc_state.bin_list_small);
	v = v && bin_verify_contiguous_internal(g_ftmalloc_state.bin_list_medium);
	v = v && bin_verify_contiguous_internal(g_ftmalloc_state.bin_list_large);
	if (!v)
	{
		ft_putendl_fd(ERR_MSG, 2);
		FTMALLOC_ASSERT(ERR_MSG);
	}
	return (v);
}
