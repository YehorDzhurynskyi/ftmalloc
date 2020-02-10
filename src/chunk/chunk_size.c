/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk_size.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydzhuryn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/15 15:08:24 by ydzhuryn          #+#    #+#             */
/*   Updated: 2018/01/05 17:20:48 by ydzhuryn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftmalloc_internal.h"

void	chunk_size_set(t_mem_chunk *chunk, size_t size)
{
	FTMALLOC_ASSERT(chunk != NULL);
	FTMALLOC_ASSERT(FTMALLOC_CHUNK_SZ_OK(size));
	chunk->size = (size & ~FTMALLOC_SERVICE_BITS) |
	(chunk->size & FTMALLOC_SERVICE_BITS);
	chunk_adj_next(chunk)->prev_size = size;
}

size_t	chunk_size_get(const t_mem_chunk *chunk)
{
	size_t size;

	FTMALLOC_ASSERT(chunk != NULL);
	size = chunk->size & ~FTMALLOC_SERVICE_BITS;
	return (size);
}
