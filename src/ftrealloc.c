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

static t_bool	try_satisfy_size(t_mem *mem, size_t size)
{
	size_t	oldsize;

	bin_verify(mem->bin);
	chunk_verify(mem->chunk);
	FTMALLOC_ASSERT(mem->bin == chunk_bin_of_slow(mem->chunk));
	FTMALLOC_ASSERT(chunk_in_use_get(mem->chunk));
	oldsize = chunk_size_get(mem->chunk);
	size = FTMALLOC_MEM_ALGN_UP(size);
	if (size == oldsize)
	{
		return (TRUE);
	}
	else if (size > oldsize)
	{
		return (realloc_try_grow(mem, size, oldsize));
	}
	else if (size < oldsize)
	{
		return (realloc_try_shrink(mem, size, oldsize));
	}
	FTMALLOC_ASSERT(!"Unhandled branch");
	return (FALSE);
}

static void		*relocate(t_mem *mem, const size_t size)
{
	void		*raw;
	size_t		osize;
	size_t		copiedsize;

	raw = ftmalloc_internal(size);
	if (raw != NULL)
	{
		osize = chunk_size_get(mem->chunk);
		copiedsize = size < osize ? size : osize;
		ft_memcpy(raw, chunk_chunk2mem(mem->chunk), copiedsize);
		FTMALLOC_DEBUG_ONLY(g_ftmalloc_state.total_alloc_copied += copiedsize);
		ftfree_internal(chunk_chunk2mem(mem->chunk));
	}
	return (raw);
}

void			*ftrealloc_internal(void *oldmem, const size_t size)
{
	t_mem	mem;
	void	*raw;

	if (oldmem == NULL)
		return (ftmalloc_internal(size));
	if (size == 0)
	{
		ftfree_internal(oldmem);
		return (NULL);
	}
	if (!mem_lookup(&mem, oldmem))
		return (NULL);
	if (try_satisfy_size(&mem, size))
	{
		raw = oldmem;
		FTMALLOC_DEBUG_ONLY(g_ftmalloc_state.total_realloc_hits++);
	}
	else
	{
		raw = relocate(&mem, size);
	}
	FTMALLOC_DEBUG_ONLY(g_ftmalloc_state.usage_realloc += raw ? 1 : 0);
	return (raw);
}

void			*ftrealloc(void *oldmem, size_t size)
{
	void	*mem;

	if (FTMALLOC_LOCK != 0 && !ftmalloc_validate_heap_if_enabled())
		return (NULL);
	mem = ftrealloc_internal(oldmem, size);
	FTMALLOC_DEBUG_ONLY(ftmalloc_check_heap_relaxed());
	FTMALLOC_UNLOCK;
	return (mem);
}

#ifdef FTMALLOC_POSIX_API

void			*realloc(void *oldmem, size_t size)
{
	return (ftrealloc(oldmem, size));
}

#endif
