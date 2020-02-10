/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk_mem.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydzhuryn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/15 15:08:24 by ydzhuryn          #+#    #+#             */
/*   Updated: 2018/01/05 17:20:48 by ydzhuryn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftmalloc_internal.h"

t_bool		chunk_is_splittable(const size_t oldsize, const size_t newsize)
{
	FTMALLOC_ASSERT(oldsize >= newsize);
	return (oldsize - newsize > FTMALLOC_MIN_SZ);
}

t_bool		chunk_is_prev_top(const t_mem_chunk *chunk)
{
	FTMALLOC_ASSERT(chunk != NULL);
	FTMALLOC_ASSERT(FTMALLOC_ALGN_OK(chunk));
	return (chunk->prev_size == 0 && chunk_prev_in_use_get(chunk));
}

t_bool		chunk_is_next_bottom(const t_mem_chunk *chunk)
{
	FTMALLOC_ASSERT(chunk != NULL);
	FTMALLOC_ASSERT(FTMALLOC_ALGN_OK(chunk));
	return (chunk_size_get(chunk_adj_next(chunk)) == 0);
}

t_byte		*chunk_chunk2mem(t_mem_chunk *chunk)
{
	t_byte	*mem;

	FTMALLOC_ASSERT(chunk != NULL);
	mem = (t_byte*)chunk + FTMALLOC_CHUNK_SZ;
	FTMALLOC_ASSERT(FTMALLOC_ALGN_OK(mem));
	return (mem);
}

t_mem_chunk	*chunk_mem2chunk(t_byte *mem)
{
	t_mem_chunk	*chunk;

	FTMALLOC_ASSERT(mem != NULL);
	chunk = (t_mem_chunk*)((t_byte*)mem - FTMALLOC_CHUNK_SZ);
	FTMALLOC_ASSERT(FTMALLOC_ALGN_OK(chunk));
	return (chunk);
}
