/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftmalloc_common.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydzhuryn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/15 15:08:24 by ydzhuryn          #+#    #+#             */
/*   Updated: 2018/01/05 17:20:48 by ydzhuryn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftmalloc_internal.h"

t_bool	ftmalloc_size_request_is_out_of_range(size_t size)
{
	size_t minsize;

	minsize = FTMALLOC_MEM_ALGN_UP(FTMALLOC_CHUNK_SZ + FTMALLOC_MIN_SZ);
	return (size >= (size_t)(-2 * minsize));
}

void	ftmalloc_call_epilogue(void)
{
	FTMALLOC_DEBUG_ONLY(ftmalloc_check_heap_relaxed());
	if (getenv(FTMALLOC_ENV_CHECK_HEAP_FULLY))
	{
		if (!ftmalloc_check_heap_fully())
		{
			errno = ENOMEM;
		}
	}
	else if (getenv(FTMALLOC_ENV_CHECK_HEAP_RELAXED))
	{
		if (!ftmalloc_check_heap_relaxed())
		{
			errno = ENOMEM;
		}
	}
}
