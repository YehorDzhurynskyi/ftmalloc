/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buddy_merge.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydzhuryn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/15 15:08:24 by ydzhuryn          #+#    #+#             */
/*   Updated: 2018/01/05 17:20:48 by ydzhuryn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftmalloc_internal.h"

static void	chunk_merge(t_mem_chunk *from, t_mem_chunk *to)
{
	chunk_verify(from);
	chunk_verify(to);
	FTMALLOC_ASSERT(to == chunk_adj_prev(from));
	FTMALLOC_ASSERT(from == chunk_adj_next(to));
	// TODO: rewrite with chunk_arange()
	const t_bool inuse = chunk_in_use_get(to);
	chunk_size_set(to, chunk_size_get(to) + chunk_size_get(from) + FTMALLOC_MEM_CHUNK_SZ);
	if (inuse)
	{
		chunk_in_use_set_true(to);
	}
	else
	{
		chunk_in_use_set_false(to);
	}
	chunk_verify(to);
}

t_mem_chunk	*buddy_try_merge_next(t_mem *mem)
{
	t_mem_chunk *merged;

	chunk_verify(mem->chunk);
	merged = NULL;
	if (!chunk_is_next_bottom(mem->chunk))
	{
		merged = chunk_adj_next(mem->chunk);
		if (chunk_in_use_get(merged))
		{
			return (NULL);
		}
		chunk_merge(merged, mem->chunk);
	}
	chunk_verify(mem->chunk);
	return (merged);
}

t_mem_chunk	*buddy_try_merge_prev(t_mem *mem)
{
	t_mem_chunk *merged;
	t_mem_chunk *prev;

	chunk_verify(mem->chunk);
	merged = NULL;
	if (!chunk_is_prev_top(mem->chunk) && !chunk_prev_in_use_get(mem->chunk))
	{
		merged = mem->chunk;
		prev = chunk_adj_prev(mem->chunk);
		chunk_merge(mem->chunk, prev);
		mem->chunk = prev;
	}
	chunk_verify(mem->chunk);
	return (merged);
}
