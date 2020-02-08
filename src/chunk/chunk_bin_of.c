/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk_bin_of.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydzhuryn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/15 15:08:24 by ydzhuryn          #+#    #+#             */
/*   Updated: 2018/01/05 17:20:48 by ydzhuryn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftmalloc_internal.h"

t_mem_bin	*chunk_bin_of(const t_mem_chunk *chunk, t_mem_bin **prev_bin)
{
	t_mem_bin	*bin;

	chunk_verify(chunk);
	bin = _bin_list_of(chunk_size_get(chunk) + FTMALLOC_MEM_CHUNK_SZ);
	if (prev_bin != NULL)
		*prev_bin = NULL;
	bin_verify(bin);
	while (bin)
	{
		bin_verify(bin);
		if (chunk >= (t_mem_chunk*)((t_byte*)bin - bin->mem_allocated) &&
			chunk < (t_mem_chunk*)bin)
		{
			break ;
		}
		if (prev_bin != NULL)
			*prev_bin = bin;
		bin = bin->next;
	}
	return (bin);
}

t_mem_bin	*chunk_bin_of_slow(const t_mem_chunk *chunk)
{
	while (!chunk_is_next_bottom(chunk))
	{
		chunk = chunk_adj_next(chunk);
	}
	chunk = chunk_adj_next(chunk);
	return ((t_mem_chunk*)((t_byte*)chunk + FTMALLOC_MEM_CHUNK_SZ));
}
