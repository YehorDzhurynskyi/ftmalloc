/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc_shrink.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydzhuryn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/15 15:08:24 by ydzhuryn          #+#    #+#             */
/*   Updated: 2018/01/05 17:20:48 by ydzhuryn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftmalloc_internal.h"

static void	realloc_shrink(t_mem *mem, size_t size, size_t osize)
{
	t_mem	adjmem;
	size_t	dsize;

	chunk_occupy(mem, size + FTMALLOC_MEM_CHUNK_SZ);
	adjmem.bin = mem->bin;
	adjmem.chunk = chunk_adj_next(mem->chunk);
	chunk_deoccupy(&adjmem);
	FTMALLOC_ASSERT(osize >= chunk_size_get(mem->chunk));
	dsize = osize - chunk_size_get(mem->chunk);
	mem->bin->mem_user -= dsize;
	mem->bin->mem_occupied -= dsize;
	FTMALLOC_DEBUG_ONLY(g_ftmalloc_state.total_dealloc += dsize);
	bin_verify(mem->bin);
	chunk_verify(mem->chunk);
	FTMALLOC_ASSERT(mem->bin == _chunk_bin_of(mem->chunk, NULL) &&
	mem->bin == _chunk_bin_of_slow(mem->chunk));
}

t_bool		realloc_try_shrink(t_mem *mem, size_t size, size_t osize)
{
	if (osize + FTMALLOC_MEM_CHUNK_SZ > _bin_max_size_of(size))
	{
		return (FALSE);
	}
	if (!_chunk_is_splittable(_chunk_size_get(mem->chunk), size))
	{
		return (TRUE);
	}
	realloc_shrink(mem, size, osize);
	return (TRUE);
}
