/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem_inpool.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydzhuryn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/15 15:08:24 by ydzhuryn          #+#    #+#             */
/*   Updated: 2018/01/05 17:20:48 by ydzhuryn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftmalloc_internal.h"

static t_bool inbucket(t_mem_bin *bin, void *mem)
{
	while (bin)
	{
		bin_verify(bin);
		if (mem >= bin_adj_top(bin) && mem < (t_mem_chunk*)bin)
		{
			return (TRUE);
		}
		bin = bin->next;
	}
	return (FALSE);
}

t_bool	mem_inpool(void *mem)
{
	t_bool inpool;

	inpool = FALSE;
	inpool = inpool || inbucket(g_ftmalloc_state.bin_list_small, mem);
	inpool = inpool || inbucket(g_ftmalloc_state.bin_list_medium, mem);
	inpool = inpool || inbucket(g_ftmalloc_state.bin_list_large, mem);
	return (inpool);
}
