/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buddy_occupy.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydzhuryn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/15 15:08:24 by ydzhuryn          #+#    #+#             */
/*   Updated: 2018/01/05 17:20:48 by ydzhuryn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftmalloc_internal.h"

static void	rebind_freed_links(t_mem *m, t_mem_chunk *next)
{
	if (!_chunk_in_use_get(m->chunk))
	{
		chunk_freed_next_set(next, m->chunk->freed_next);
		chunk_freed_prev_set(next, m->chunk->freed_prev);
	}
	else
	{
		chunk_freed_next_set(next, NULL);
		chunk_freed_prev_set(next, NULL);
	}
	if (m->bin->head == m->chunk)
	{
		FTMALLOC_ASSERT(chunk_freed_prev_get(m->bin->head) == NULL);
		m->bin->head = next;
	}
}

static void	chunk_split(t_mem *m, const size_t s)
{
	t_mem_chunk	*next;
	size_t		osize;

	chunk_verify(m->chunk);
	FTMALLOC_ASSERT(FTMALLOC_MEM_CHUNK_SZ_OK(s));
	FTMALLOC_ASSERT(chunk_size_get(m->chunk) >= s + FTMALLOC_MEM_CHUNK_SZ);
	osize = chunk_size_get(m->chunk);
	m->chunk = chunk_arrange(m->chunk, s - FTMALLOC_MEM_CHUNK_SZ,
	chunk_in_use_get(m->chunk));
	next = chunk_arrange(chunk_adj_next(m->chunk), osize - s, FALSE);
	FTMALLOC_ASSERT(!chunk_in_use_get(next));
	rebind_freed_links(m, next);
	m->bin->mem_occupied += FTMALLOC_MEM_CHUNK_SZ;
	chunk_verify(next);
	FTMALLOC_ASSERT(chunk_adj_next(m->chunk) == next);
	FTMALLOC_ASSERT(chunk_adj_prev(next) == m->chunk);
}

void		buddy_occupy(t_mem *m, const size_t s)
{
	chunk_verify(m->chunk);
	FTMALLOC_ASSERT(FTMALLOC_MEM_CHUNK_SZ_OK(s));
	FTMALLOC_ASSERT(chunk_size_get(m->chunk) >= s - FTMALLOC_MEM_CHUNK_SZ);
	if (chunk_is_splittable(chunk_size_get(m->chunk),
	s - FTMALLOC_MEM_CHUNK_SZ))
	{
		chunk_split(m, s);
	}
	else
	{
		FTMALLOC_ASSERT(
			chunk_size_get(m->chunk) - (s - FTMALLOC_MEM_CHUNK_SZ) == 0 ||
			chunk_size_get(m->chunk) -
			(s - FTMALLOC_MEM_CHUNK_SZ) == FTMALLOC_MEM_MIN_PAYLOAD_SZ);
		FTMALLOC_ASSERT(chunk_size_get(m->chunk) +
		FTMALLOC_MEM_CHUNK_SZ <= bin_max_size_of(s));
	}
	chunk_in_use_set_true(m->chunk);
	chunk_verify(m->chunk);
}
