/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk_freed.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydzhuryn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/15 15:08:24 by ydzhuryn          #+#    #+#             */
/*   Updated: 2018/01/05 17:20:48 by ydzhuryn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftmalloc_internal.h"

t_mem_chunk	*chunk_freed_prev_get(const t_mem_chunk *chunk)
{
	t_mem_chunk *freed_prev;

	FTMALLOC_ASSERT(chunk != NULL);
	FTMALLOC_ASSERT(!_chunk_in_use_get(chunk));
	freed_prev = chunk->freed_prev;
	FTMALLOC_ASSERT(!freed_prev || !_chunk_in_use_get(freed_prev));
	return (freed_prev);
}

t_mem_chunk	*chunk_freed_next_get(const t_mem_chunk *chunk)
{
	t_mem_chunk *freed_next;

	FTMALLOC_ASSERT(chunk != NULL);
	FTMALLOC_ASSERT(!_chunk_in_use_get(chunk));
	freed_next = chunk->freed_next;
	FTMALLOC_ASSERT(!freed_next || !_chunk_in_use_get(freed_next));
	return (freed_next);
}

void		chunk_freed_prev_set(t_mem_chunk *chunk, t_mem_chunk *freed)
{
	FTMALLOC_ASSERT(chunk != NULL);
	FTMALLOC_ASSERT(chunk != freed);
	FTMALLOC_ASSERT(!_chunk_in_use_get(chunk));
	FTMALLOC_ASSERT(!freed || !_chunk_in_use_get(freed));
	chunk->freed_prev = freed;
	if (freed)
	{
		freed->freed_next = chunk;
	}
}

void		chunk_freed_next_set(t_mem_chunk *chunk, t_mem_chunk *freed)
{
	FTMALLOC_ASSERT(chunk != NULL);
	FTMALLOC_ASSERT(chunk != freed);
	FTMALLOC_ASSERT(!_chunk_in_use_get(chunk));
	FTMALLOC_ASSERT(!freed || !_chunk_in_use_get(freed));
	chunk->freed_next = freed;
	if (freed)
	{
		freed->freed_prev = chunk;
	}
}
