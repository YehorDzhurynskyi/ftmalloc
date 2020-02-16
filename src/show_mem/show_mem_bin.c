/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_mem_bin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydzhuryn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/15 15:08:24 by ydzhuryn          #+#    #+#             */
/*   Updated: 2018/01/05 17:20:48 by ydzhuryn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftmalloc_internal.h"

void		show_mem_bin(t_mem_bin *bin, t_show_chunk_func show_func)
{
	t_mem_chunk* chunk;

	bin_verify(bin);
	ft_putstr("\t");
	ft_putnbr(bin->mem_occupied);
	ft_putstr(" Occupied(");
	ft_putnbr(bin->mem_user);
	ft_putstr(" User, ");
	ft_putnbr(FTMALLOC_BIN_SZ);
	ft_putstr(" Bin Header) / ");
	ft_putnbr(bin->mem_allocated - bin->mem_occupied);
	ft_putstr(" Available / ");
	ft_putnbr(bin->mem_allocated);
	ft_putstr(" Total\n\tHead: ");
	ft_putaddress((void*)bin->head);
	ft_putstr("\n\tNext Bin: ");
	ft_putaddress((void*)bin->next);
	ft_putstr("\n");
	chunk = (t_mem_chunk*)((t_byte*)bin - (bin->mem_allocated - FTMALLOC_BIN_HEADER_SZ - FTMALLOC_CHUNK_SZ));
	FTMALLOC_ASSERT(chunk_is_prev_top(chunk));
	while (!chunk_is_next_bottom(chunk))
	{
		show_func(chunk);
		chunk = chunk_adj_next(chunk);
	}
	show_func(chunk);
	ft_putstr("\n");
}

static void	show_mem_bucket(t_show_chunk_func show_func, t_mem_bin *bin, const char *bucketname)
{
	ft_putstr(bucketname);
	ft_putstr(bin ? ":\n" : " (Empty)\n");
	while (bin)
	{
		show_mem_bin(bin, show_func);
		bin = bin->next;
	}
}

void		show_mem_internal(t_show_chunk_func show_func)
{
	ft_putstr(" === HEAP USAGE ===\n");
	show_mem_bucket(show_func, g_ftmalloc_state.bin_list_small, "Small Bucket");
	show_mem_bucket(show_func, g_ftmalloc_state.bin_list_medium, "Medium Bucket");
	show_mem_bucket(show_func, g_ftmalloc_state.bin_list_large, "Large Bucket");
}
