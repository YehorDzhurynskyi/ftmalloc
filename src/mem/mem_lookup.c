/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem_lookup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydzhuryn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/15 15:08:24 by ydzhuryn          #+#    #+#             */
/*   Updated: 2018/01/05 17:20:48 by ydzhuryn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftmalloc_internal.h"

static t_bool	chunk_lookup(t_mem_bin *bin, t_mem *mem, void *raw)
{
	t_mem_chunk	*chunk;

	chunk = bin_adj_bottom(bin);
	while (!chunk_is_prev_top(chunk))
	{
		chunk_verify(chunk);
		if (chunk_chunk2mem(chunk) == raw)
		{
			mem->chunk = chunk;
			return (TRUE);
		}
		chunk = chunk_adj_prev(chunk);
	}
	chunk_verify(chunk);
	if (chunk_chunk2mem(chunk) == raw)
	{
		mem->chunk = chunk;
		return (TRUE);
	}
	return (FALSE);
}

static t_bool	bin_lookup(t_mem_bin *bin, t_mem *mem, void *raw)
{
	while (bin)
	{
		bin_verify(bin);
		if (raw >= (void*)bin_adj_top(bin) && raw < (void*)bin)
		{
			mem->bin = bin;
			return (chunk_lookup(bin, mem, raw));
		}
		bin = bin->next;
	}
	return (FALSE);
}

t_bool			mem_lookup(t_mem *mem, void *raw)
{
	t_bool inpool;

	inpool = FALSE;
	inpool = inpool || bin_lookup(g_ftmalloc_state.bin_list_small, mem, raw);
	inpool = inpool || bin_lookup(g_ftmalloc_state.bin_list_medium, mem, raw);
	inpool = inpool || bin_lookup(g_ftmalloc_state.bin_list_large, mem, raw);
	return (inpool);
}
