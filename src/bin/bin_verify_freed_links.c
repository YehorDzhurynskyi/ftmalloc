/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bin_verify_freed_links.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydzhuryn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/15 15:08:24 by ydzhuryn          #+#    #+#             */
/*   Updated: 2018/01/05 17:20:48 by ydzhuryn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftmalloc_internal.h"

static void	bin_verify_freed_links_internal(t_mem_bin *bin)
{
	t_mem_chunk	*prev;
	t_mem_chunk	*chunk;

	while (bin)
	{
		prev = NULL;
		chunk = bin->head;
		while (chunk)
		{
			FTMALLOC_ASSERT(!chunk_in_use_get(chunk));
			FTMALLOC_ASSERT(prev == chunk->freed_prev);
			prev = chunk;
			FTMALLOC_ASSERT(chunk != chunk->freed_next);
			chunk = chunk->freed_next;
		}
		bin = bin->next;
	}
}

void		bin_verify_freed_links(void)
{
	bin_verify_freed_links_internal(g_ftmalloc_state.bin_list_small);
	bin_verify_freed_links_internal(g_ftmalloc_state.bin_list_medium);
	bin_verify_freed_links_internal(g_ftmalloc_state.bin_list_large);
}
