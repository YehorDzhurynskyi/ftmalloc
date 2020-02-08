/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftmalloc_show_mem.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydzhuryn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/15 15:08:24 by ydzhuryn          #+#    #+#             */
/*   Updated: 2018/01/05 17:20:48 by ydzhuryn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftmalloc_internal.h"

static void	show_bin(const t_mem_bin *bin, t_show_chunk_func show_func)
{
	t_mem_chunk *chunk;

	bin_verify(bin);
	ft_printf("\t%zu Occupied(%zu User, %zu Bin Header)"
				" / %zu Available / %zu Total\n",
				bin->mem_occupied,
				bin->mem_user,
				FTMALLOC_MEM_BIN_SZ,
				bin->mem_allocated - bin->mem_occupied,
				bin->mem_allocated);
	ft_printf("\tHead: %p\n", bin->head);
	ft_printf("\tNext Bin: %p\n", bin->next);
	chunk = bin_adj(bin);
	while (!chunk_is_prev_top(chunk))
	{
		show_func(chunk);
		chunk = chunk_adj_prev(chunk);
	}
	show_func(chunk);
	ft_printf("\n");
}

static void	show_mem_internal(t_show_chunk_func show_func)
{
	t_mem_bin *bin;

	ft_printf(" === HEAP USAGE ===\n");
	bin = g_ftmalloc_state.bin_list_small;
	ft_printf("Small Bucket%s\n", bin ? ":" : " (Empty)");
	while (bin)
	{
		show_bin(bin, show_func);
		bin = bin->next;
	}
	bin = g_ftmalloc_state.bin_list_medium;
	ft_printf("Medium Bucket%s\n", bin ? ":" : " (Empty)");
	while (bin)
	{
		show_bin(bin, show_func);
		bin = bin->next;
	}
	bin = g_ftmalloc_state.bin_list_large;
	ft_printf("Large Bucket%s\n", bin ? ":" : " (Empty)");
	while (bin)
	{
		show_bin(bin, show_func);
		bin = bin->next;
	}
}

void		ftmalloc_show_mem(void)
{
	show_mem_internal(show_mem_chunk);
}

void		ftmalloc_show_mem_ex(void)
{
	show_mem_internal(show_mem_chunk_ex);
	FTMALLOC_DEBUG_ONLY(
	ft_printf("\nTotal Heap Usage:\n\t%i mmap\n\t%i munmap\n\t%i"
				" allocs\n\t%i deallocs\n\t%i reallocs(%i hits)\n\t"
				"%zu bytes allocated(%zu copied)\n\t%zu bytes deallocated\n",
				g_ftmalloc_state.total_mmap,
				g_ftmalloc_state.total_munmap,
				g_ftmalloc_state.usage_alloc,
				g_ftmalloc_state.usage_dealloc,
				g_ftmalloc_state.usage_realloc,
				g_ftmalloc_state.total_realloc_hits,
				g_ftmalloc_state.total_alloc,
				g_ftmalloc_state.total_alloc_copied,
				g_ftmalloc_state.total_dealloc));
}
