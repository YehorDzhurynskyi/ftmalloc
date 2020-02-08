/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem_find.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydzhuryn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/15 15:08:24 by ydzhuryn          #+#    #+#             */
/*   Updated: 2018/01/05 17:20:48 by ydzhuryn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftmalloc_internal.h"

static size_t	check_bucket_overflow(t_mem *mem, const size_t size)
{
	size_t oldsize;
	size_t newsize;

	oldsize = chunk_size_get(mem->chunk);
	newsize = size - FTMALLOC_MEM_CHUNK_SZ;
	if (oldsize != newsize && !chunk_is_splittable(oldsize, newsize))
	{
		FTMALLOC_ASSERT(oldsize - newsize == FTMALLOC_MEM_MIN_PAYLOAD_SZ);
		if (oldsize + FTMALLOC_MEM_CHUNK_SZ > bin_max_size_of(size))
		{
			return (mem_reserve(mem, size + FTMALLOC_MEM_CHUNK_SZ));
		}
	}
	return (size);
}

size_t			mem_find(t_mem *mem, const size_t size)
{
	t_mem_bin	*bin;
	t_mem_chunk	*chunk;

	bin = bin_list_of(size);
	while (bin != NULL)
	{
		bin_verify(bin);
		if (bin->mem_allocated - bin->mem_occupied >= size)
		{
			chunk = bin->head;
			while (chunk != NULL)
			{
				chunk_verify(chunk);
				if (chunk_size_get(chunk) >= size - FTMALLOC_MEM_CHUNK_SZ)
				{
					mem->chunk = chunk;
					mem->bin = bin;
					return (check_bucket_overflow(mem, size));
				}
				chunk = chunk_freed_next_get(chunk);
			}
		}
		bin = bin->next;
	}
	return (0);
}
