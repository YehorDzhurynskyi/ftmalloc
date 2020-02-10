/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bin_of.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydzhuryn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/15 15:08:24 by ydzhuryn          #+#    #+#             */
/*   Updated: 2018/01/05 17:20:48 by ydzhuryn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftmalloc_internal.h"

size_t		bin_max_size_of(size_t size)
{
	if (size <= FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_SMALL)
	{
		return (FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_SMALL);
	}
	else if (size <= FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_MEDIUM)
	{
		return (FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_MEDIUM);
	}
	return (-1);
}

t_mem_bin	*bin_list_of(size_t size)
{
	FTMALLOC_ASSERT(FTMALLOC_CHUNK_SZ_OK(size));
	if (size <= FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_SMALL)
	{
		return (g_ftmalloc_state.bin_list_small);
	}
	else if (size <= FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_MEDIUM)
	{
		return (g_ftmalloc_state.bin_list_medium);
	}
	return (g_ftmalloc_state.bin_list_large);
}

size_t		bin_alloc_size_of(size_t size)
{
	size_t	multiple;
	size_t	alloc_size;

	FTMALLOC_ASSERT(FTMALLOC_CHUNK_SZ_OK(size));
	multiple = getpagesize();
	alloc_size = 0;
	if (size <= FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_SMALL)
	{
		alloc_size = FTMALLOC_BIN_APPROX_MIN_NITEMS *
		FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_SMALL;
	}
	else if (size <= FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_MEDIUM)
	{
		alloc_size = FTMALLOC_BIN_APPROX_MIN_NITEMS *
		FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_MEDIUM;
	}
	else
	{
		alloc_size = ft_ceil((size + FTMALLOC_BIN_SZ) /
		(float)multiple) * multiple;
	}
	alloc_size = FTMALLOC_ALGN_UP(alloc_size, multiple);
	FTMALLOC_ASSERT(FTMALLOC_CHUNK_SZ_OK(alloc_size));
	FTMALLOC_ASSERT(alloc_size % multiple == 0);
	return (alloc_size);
}
