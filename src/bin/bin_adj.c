/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bin_adj.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydzhuryn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/15 15:08:24 by ydzhuryn          #+#    #+#             */
/*   Updated: 2018/01/05 17:20:48 by ydzhuryn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftmalloc_internal.h"

t_mem_chunk	*bin_adj(const t_mem_bin *bin)
{
	t_mem_chunk *bottom_chunk;

	bin_verify(bin);
	bottom_chunk = (t_mem_chunk*)((t_byte*)bin - FTMALLOC_CHUNK_SZ);
	return ((t_mem_chunk*)((t_byte*)bottom_chunk -
	bottom_chunk->prev_size - FTMALLOC_CHUNK_SZ));
}
