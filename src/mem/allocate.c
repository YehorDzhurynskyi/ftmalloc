/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem_allocate.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydzhuryn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/15 15:08:24 by ydzhuryn          #+#    #+#             */
/*   Updated: 2018/01/05 17:20:48 by ydzhuryn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftmalloc_internal.h"

static void	fill_with_wildcards(t_byte *raw, const size_t size)
{
	int i;

	i = 0;
	while (i < size)
	{
		raw[i++] = (char)(rand() % 256);
	}
}

static void	bind_to_list(t_mem_bin *bin, const size_t size)
{
	t_mem_bin	*bin_list;

	bin_list = bin_list_of(size);
	if (bin_list != NULL)
	{
		while (bin_list->next)
			bin_list = bin_list->next;
		bin_list->next = bin;
	}
	else
	{
		if (size <= FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_SMALL)
		{
			g_ftmalloc_state.bin_list_small = bin;
		}
		else if (size <= FTMALLOC_BIN_ITEM_MAX_ALLOC_SIZE_MEDIUM)
		{
			g_ftmalloc_state.bin_list_medium = bin;
		}
		else
		{
			g_ftmalloc_state.bin_list_large = bin;
		}
	}
}

size_t		mem_allocate(t_mem *mem, const size_t size)
{
	t_mem_bin	*bin;
	t_byte		*raw;
	size_t		alloc_size;

	alloc_size = _bin_alloc_size_of(size);
	raw = mmap(NULL,
	alloc_size,
	PROT_READ | PROT_WRITE,
	MAP_PRIVATE | MAP_ANONYMOUS,
	-1,
	0);
	if (raw == MAP_FAILED)
	{
		return (0);
	}
	FTMALLOC_DEBUG_ONLY(g_ftmalloc_state.total_mmap++);
	FTMALLOC_DEBUG_ONLY(fill_with_wildcards(raw, alloc_size));
	FTMALLOC_ASSERT(FTMALLOC_MEM_ALIGNED_OK(raw));
	bin = bin_init(raw, alloc_size);
	bind_to_list(bin, size);
	mem->bin = bin;
	mem->chunk = bin->head;
	return (size);
}
