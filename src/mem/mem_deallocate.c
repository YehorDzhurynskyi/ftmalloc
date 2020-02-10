/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem_deallocate.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydzhuryn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/15 15:08:24 by ydzhuryn          #+#    #+#             */
/*   Updated: 2018/01/05 17:20:48 by ydzhuryn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftmalloc_internal.h"

static void	unbind_from_list(t_mem *mem, const size_t size)
{
	if (size + FTMALLOC_CHUNK_SZ <=
	FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_SMALL)
	{
		g_ftmalloc_state.bin_list_small = mem->bin->next;
	}
	else if (size + FTMALLOC_CHUNK_SZ <=
	FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_MEDIUM)
	{
		g_ftmalloc_state.bin_list_medium = mem->bin->next;
	}
	else
	{
		g_ftmalloc_state.bin_list_large = mem->bin->next;
	}
}

void		mem_deallocate(t_mem *mem, const size_t size, t_mem_bin *prev_bin)
{
	bin_verify(mem->bin);
	FTMALLOC_ASSERT(bin_is_empty(mem->bin));
	if (prev_bin == NULL)
	{
		unbind_from_list(mem, size);
	}
	else
	{
		prev_bin->next = mem->bin->next;
	}
	if (munmap(bin_bin2mem(mem->bin), mem->bin->mem_allocated) == -1)
	{
		FTMALLOC_ASSERT(!"Unexpected fail or invalid param");
	}
	FTMALLOC_DEBUG_ONLY(g_ftmalloc_state.total_munmap++);
}
