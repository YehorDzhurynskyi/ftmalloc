/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk_verify.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydzhuryn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/15 15:08:24 by ydzhuryn          #+#    #+#             */
/*   Updated: 2018/01/05 17:20:48 by ydzhuryn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftmalloc_internal.h"

void	chunk_verify(const t_mem_chunk *chunk)
{
	FTMALLOC_ASSERT(chunk);
	FTMALLOC_ASSERT(FTMALLOC_MEM_ALIGNED_OK(chunk));
	FTMALLOC_ASSERT(FTMALLOC_MEM_CHUNK_SZ_OK(chunk_size_get(chunk)));
	FTMALLOC_ASSERT(chunk_size_get(chunk) == chunk_adj_next(chunk)->prev_size);
	FTMALLOC_ASSERT(chunk->prev_size == chunk_size_get(chunk_adj_prev(chunk)));
	if (!chunk_is_prev_top(chunk))
	{
		FTMALLOC_ASSERT(FTMALLOC_MEM_CHUNK_SZ_OK(chunk->prev_size));
	}
	if (!chunk_in_use_get(chunk))
	{
		if (chunk_freed_next_get(chunk) != NULL)
		{
			FTMALLOC_ASSERT(chunk_freed_next_get(chunk)->freed_prev == chunk);
			FTMALLOC_ASSERT(!chunk_in_use_get(chunk_freed_next_get(chunk)));
		}
		if (chunk_freed_prev_get(chunk) != NULL)
		{
			FTMALLOC_ASSERT(chunk_freed_prev_get(chunk)->freed_next == chunk);
			FTMALLOC_ASSERT(!chunk_in_use_get(chunk_freed_prev_get(chunk)));
		}
	}
}
