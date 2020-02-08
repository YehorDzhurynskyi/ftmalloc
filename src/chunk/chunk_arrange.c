/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk_arrange.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydzhuryn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/15 15:08:24 by ydzhuryn          #+#    #+#             */
/*   Updated: 2018/01/05 17:20:48 by ydzhuryn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftmalloc_internal.h"

t_mem_chunk	*chunk_arrange(t_byte *mem, size_t size, t_bool inuse)
{
	t_mem_chunk *chunk;

	FTMALLOC_ASSERT(mem != NULL);
	FTMALLOC_ASSERT(FTMALLOC_MEM_ALIGNED_OK(mem));
	FTMALLOC_ASSERT(FTMALLOC_MEM_CHUNK_SZ_OK(size));
	chunk = (t_mem_chunk*)mem;
	_chunk_size_set(chunk, size);
	if (inuse)
	{
		_chunk_in_use_set_true(chunk);
	}
	else
	{
		_chunk_in_use_set_false(chunk);
	}
	return (chunk);
}
