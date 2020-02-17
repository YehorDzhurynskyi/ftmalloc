/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftmalloc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydzhuryn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/15 15:08:24 by ydzhuryn          #+#    #+#             */
/*   Updated: 2018/01/05 17:20:48 by ydzhuryn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftmalloc_internal.h"

static t_bool	validate_input(size_t num, size_t size)
{
	FTMALLOC_ASSERT(FTMALLOC_ALGN_OK(FTMALLOC_CHUNK_SZ));
	FTMALLOC_ASSERT(FTMALLOC_ALGN_OK(FTMALLOC_MIN_SZ));
	if (num == 0 || size == 0)
	{
		return (FALSE);
	}
	if (ftmalloc_size_request_is_out_of_range(num) ||
		ftmalloc_size_request_is_out_of_range(size))
	{
		errno = ENOMEM;
		return (FALSE);
	}
	return (TRUE);
}

void			*ftcalloc_internal(size_t num, size_t size)
{
	void *mem;

	if (!validate_input(num, size))
	{
		return (NULL);
	}
	mem = ftmalloc_internal(num * size);
	if (mem)
	{
		ft_memset(mem, 0x0, num * size);
	}
	return (mem);
}

void			*ftcalloc(size_t num, size_t size)
{
	void *mem;

	if (FTMALLOC_LOCK != 0 && !ftmalloc_validate_heap_if_enabled())
		return (NULL);
	mem = ftcalloc_internal(num, size);
	FTMALLOC_DEBUG_ONLY(ftmalloc_check_heap_relaxed());
	FTMALLOC_UNLOCK;
	return (mem);
}

#ifdef FTMALLOC_POSIX_API
void			*calloc(size_t num, size_t size)
{
	return (ftcalloc(num, size));
}
#endif
