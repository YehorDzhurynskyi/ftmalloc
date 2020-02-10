/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem_reserve.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydzhuryn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/15 15:08:24 by ydzhuryn          #+#    #+#             */
/*   Updated: 2018/01/05 17:20:48 by ydzhuryn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftmalloc_internal.h"

static void	rebind_freed_links(t_mem *mem, const size_t size)
{
	FTMALLOC_ASSERT(!chunk_in_use_get(mem->chunk));
	if (mem->bin->head == mem->chunk)
	{
		FTMALLOC_ASSERT(chunk_freed_prev_get(mem->bin->head) == NULL);
		mem->bin->head = chunk_freed_next_get(mem->bin->head);
	}
	if (mem->chunk->freed_prev != NULL)
		chunk_freed_next_set(mem->chunk->freed_prev, mem->chunk->freed_next);
	if (mem->chunk->freed_next != NULL)
		chunk_freed_prev_set(mem->chunk->freed_next, mem->chunk->freed_prev);
	chunk_freed_prev_set(mem->chunk, NULL);
	chunk_freed_next_set(mem->chunk, NULL);
}

size_t		mem_reserve(t_mem *mem, const size_t size)
{
	size_t newsize;

	FTMALLOC_ASSERT(size > 0 && FTMALLOC_ALGN_OK(size));
	if ((newsize = mem_find(mem, size)) == 0)
	{
		if ((newsize = mem_allocate(mem, size)) == 0)
		{
			errno = ENOMEM;
			return (0);
		}
	}
	if (!chunk_is_splittable(chunk_size_get(mem->chunk),
	size - FTMALLOC_CHUNK_SZ))
	{
		rebind_freed_links(mem, size);
	}
	return (newsize);
}
