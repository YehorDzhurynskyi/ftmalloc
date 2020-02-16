/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftrealloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydzhuryn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/15 15:08:24 by ydzhuryn          #+#    #+#             */
/*   Updated: 2018/01/05 17:20:48 by ydzhuryn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftmalloc_internal.h"

static t_bool	try_satisfy_size(t_byte *raw, size_t size)
{
	t_mem	mem;
	size_t	oldsize;

	mem.chunk = chunk_mem2chunk(raw);
	mem.bin = chunk_bin_of(mem.chunk, NULL);
	bin_verify(mem.bin);
	chunk_verify(mem.chunk);
	FTMALLOC_ASSERT(mem.bin == chunk_bin_of_slow(mem.chunk));
	FTMALLOC_ASSERT(chunk_in_use_get(mem.chunk));
	oldsize = chunk_size_get(mem.chunk);
	size = FTMALLOC_MEM_ALGN_UP(size);
	if (size == oldsize)
	{
		return (TRUE);
	}
	else if (size > oldsize)
	{
		return (realloc_try_grow(&mem, size, oldsize));
	}
	else if (size < oldsize)
	{
		return (realloc_try_shrink(&mem, size, oldsize));
	}
	FTMALLOC_ASSERT(!"Unhandled branch");
	return (FALSE);
}

static void		*relocate(void *oldmem, const size_t size)
{
	void		*mem;
	t_mem_chunk	*oldchunk;
	size_t		osize;
	size_t		copiedsize;

	mem = ftmalloc_internal(size);
	if (mem != NULL)
	{
		oldchunk = chunk_mem2chunk(oldmem);
		osize = chunk_size_get(oldchunk);
		copiedsize = size < osize ? size : osize;
		ft_memcpy(mem, oldmem, copiedsize);
		FTMALLOC_DEBUG_ONLY(g_ftmalloc_state.total_alloc_copied += copiedsize);
		ftfree_internal(oldmem);
	}
	return (mem);
}

void			*ftrealloc_internal(void *oldmem, const size_t size)
{
	void	*mem;

	if (oldmem == NULL)
	{
		return (ftmalloc_internal(size));
	}
	if (size == 0)
	{
		ftfree_internal(oldmem);
		return (NULL);
	}
	if (try_satisfy_size(oldmem, size))
	{
		mem = oldmem;
		FTMALLOC_DEBUG_ONLY(g_ftmalloc_state.total_realloc_hits++);
	}
	else
	{
		mem = relocate(oldmem, size);
	}
	FTMALLOC_DEBUG_ONLY(g_ftmalloc_state.usage_realloc += mem ? 1 : 0);
	return (mem);
}

void			*ftrealloc(void *oldmem, size_t size)
{
	void	*mem;

	if (FTMALLOC_LOCK != 0)
	{
		return (NULL);
	}
	mem = ftrealloc_internal(oldmem, size);
	ftmalloc_call_epilogue();
	FTMALLOC_UNLOCK;
	return (mem);
}

#ifdef FTMALLOC_POSIX_API
void			*realloc(void *oldmem, size_t size)
{
	return (ftrealloc(oldmem, size));
}
#endif
