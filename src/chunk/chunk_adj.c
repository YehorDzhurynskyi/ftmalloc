/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk_adj.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydzhuryn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/15 15:08:24 by ydzhuryn          #+#    #+#             */
/*   Updated: 2018/01/05 17:20:48 by ydzhuryn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftmalloc_internal.h"

t_mem_chunk	*chunk_adj_prev(const t_mem_chunk *chunk)
{
	FTMALLOC_ASSERT(chunk != NULL);
	return ((t_mem_chunk*)((t_byte*)chunk -
	FTMALLOC_CHUNK_SZ - chunk->prev_size));
}

t_mem_chunk	*chunk_adj_next(const t_mem_chunk *chunk)
{
	FTMALLOC_ASSERT(chunk != NULL);
	return ((t_mem_chunk*)((t_byte*)chunk +
	FTMALLOC_CHUNK_SZ + chunk_size_get(chunk)));
}
