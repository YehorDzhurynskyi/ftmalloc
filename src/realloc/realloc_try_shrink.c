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

	buddy_occupy(mem, size + FTMALLOC_MEM_CHUNK_SZ);
	adjmem.bin = mem->bin;
	adjmem.chunk = chunk_adj_next(mem->chunk);
	buddy_deoccupy(&adjmem);
    if (getenv(FTMALLOC_ENV_SCRIBBLE))
	    ft_memset((t_byte*)chunk_chunk2mem(adjmem.chunk) + FTMALLOC_MEM_MIN_PAYLOAD_SZ, 0xfd, chunk_size_get(adjmem.chunk) - FTMALLOC_MEM_MIN_PAYLOAD_SZ);
	FTMALLOC_ASSERT(osize >= chunk_size_get(mem->chunk));
	dsize = osize - chunk_size_get(mem->chunk);
	mem->bin->mem_user -= dsize;
	mem->bin->mem_occupied -= dsize;
	FTMALLOC_DEBUG_ONLY(g_ftmalloc_state.total_dealloc += dsize);
	bin_verify(mem->bin);
	chunk_verify(mem->chunk);
	FTMALLOC_ASSERT(mem->bin == chunk_bin_of(mem->chunk, NULL) &&
	mem->bin == chunk_bin_of_slow(mem->chunk));
}

t_bool		realloc_try_shrink(t_mem *mem, size_t size, size_t osize)
{
	if (osize + FTMALLOC_MEM_CHUNK_SZ > bin_max_size_of(size))
	{
		return (FALSE);
	}
	if (!chunk_is_splittable(chunk_size_get(mem->chunk), size))
	{
		return (TRUE);
	}
	realloc_shrink(mem, size, osize);
	return (TRUE);
}
