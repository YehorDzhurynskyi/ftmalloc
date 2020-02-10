/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftmalloc_check_heap_relaxed.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydzhuryn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/15 15:08:24 by ydzhuryn          #+#    #+#             */
/*   Updated: 2018/01/05 17:20:48 by ydzhuryn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftmalloc_internal.h"

#define ERR_MSG "ftmalloc heap corruption error: invalid freed links!"

static t_bool	bin_verify_freed_links_internal(t_mem_bin *bin)
{
	t_mem_chunk	*prev;
	t_mem_chunk	*chunk;

	while (bin)
	{
		prev = NULL;
		chunk = bin->head;
		while (chunk)
		{
			if (chunk_in_use_get(chunk) ||
				prev != chunk->freed_prev ||
				chunk == chunk->freed_next)
			{
				return (FALSE);
			}
			prev = chunk;
			chunk = chunk->freed_next;
		}
		bin = bin->next;
	}
	return (TRUE);
}

int				ftmalloc_check_heap_relaxed(void)
{
	t_bool v;

	v = TRUE;
	v = v && bin_verify_freed_links_internal(g_ftmalloc_state.bin_list_small);
	v = v && bin_verify_freed_links_internal(g_ftmalloc_state.bin_list_medium);
	v = v && bin_verify_freed_links_internal(g_ftmalloc_state.bin_list_large);
	if (!v)
	{
		ft_putendl_fd(ERR_MSG, 2);
		FTMALLOC_ASSERT(v && ERR_MSG);
	}
	return (v);
}
