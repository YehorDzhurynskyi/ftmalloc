/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_mem_chunk.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydzhuryn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/15 15:08:24 by ydzhuryn          #+#    #+#             */
/*   Updated: 2018/01/05 17:20:48 by ydzhuryn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftmalloc_internal.h"

void	show_mem_chunk(t_mem_chunk *chunk)
{
	chunk_verify(chunk);
	ft_printf("\t\tAddress: %p\n\t\tUser address: %p\n",
				chunk,
				chunk_chunk2mem(chunk));
	ft_printf("\t\tSize: %zu Occupied(%zu User) / %zu Available\n",
				(chunk_in_use_get(chunk) ? chunk_size_get(chunk) : 0) +
				FTMALLOC_CHUNK_SZ,
				chunk_in_use_get(chunk) ? chunk_size_get(chunk) : 0,
				chunk_in_use_get(chunk) ? 0 : chunk_size_get(chunk));
	ft_printf("\t\tFree: %s\n", chunk_in_use_get(chunk) ? "No" : "Yes");
	if (!chunk_in_use_get(chunk))
	{
		ft_printf("\t\tNext Chunk: %p\n", chunk_freed_next_get(chunk));
		ft_printf("\t\tPrev Chunk: %p\n", chunk_freed_prev_get(chunk));
	}
	ft_printf("\n");
}
