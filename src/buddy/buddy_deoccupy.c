/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buddy_deoccupy.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydzhuryn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/15 15:08:24 by ydzhuryn          #+#    #+#             */
/*   Updated: 2018/01/05 17:20:48 by ydzhuryn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftmalloc_internal.h"

static void	bind_closest_links(t_mem *mem)
{
	t_mem_chunk	*freed;
	t_mem_chunk	*next;

	freed = mem->bin->head;
	while ((next = _chunk_freed_next_get(freed)) != NULL)
	{
		if (next > mem->chunk)
		{
			break ;
		}
		freed = next;
	}
	if (next == NULL)
	{
		FTMALLOC_ASSERT(freed->freed_next == NULL);
		_chunk_freed_prev_set(mem->chunk, freed);
		_chunk_freed_next_set(mem->chunk, NULL);
	}
	else
	{
		FTMALLOC_ASSERT(next->freed_prev != NULL);
		_chunk_freed_prev_set(mem->chunk, next->freed_prev);
		_chunk_freed_next_set(mem->chunk, next);
	}
}

void		buddy_deoccupy(t_mem *mem)
{
	if (mem->bin->head == NULL)
	{
		chunk_freed_prev_set(mem->chunk, NULL);
		chunk_freed_next_set(mem->chunk, NULL);
		mem->bin->head = mem->chunk;
	}
	else if (mem->bin->head > mem->chunk)
	{
		chunk_freed_prev_set(mem->chunk, mem->bin->head->freed_prev);
		chunk_freed_next_set(mem->chunk, mem->bin->head);
		FTMALLOC_ASSERT(mem->chunk->freed_prev == NULL);
		FTMALLOC_ASSERT(mem->chunk->freed_next != NULL);
		mem->bin->head = mem->chunk;
	}
	else
	{
		bind_closest_links(mem);
	}
}
