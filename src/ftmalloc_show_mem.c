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

void		ftmalloc_show_mem(void)
{
	if (FTMALLOC_LOCK != 0)
	{
		return ;
	}
	show_mem_internal(show_mem_chunk);
	FTMALLOC_UNLOCK;
}

void		ftmalloc_show_mem_ex(void)
{
	if (FTMALLOC_LOCK != 0)
	{
		return;
	}
	show_mem_internal(show_mem_chunk_ex);
	FTMALLOC_DEBUG_ONLY(ft_putstr("\nTotal Heap Usage:\n\t"));
	FTMALLOC_DEBUG_ONLY(ft_putnbr(g_ftmalloc_state.total_mmap));
	FTMALLOC_DEBUG_ONLY(ft_putstr(" mmap\n\t"));
	FTMALLOC_DEBUG_ONLY(ft_putnbr(g_ftmalloc_state.total_munmap));
	FTMALLOC_DEBUG_ONLY(ft_putstr(" munmap\n\t"));
	FTMALLOC_DEBUG_ONLY(ft_putnbr(g_ftmalloc_state.usage_alloc));
	FTMALLOC_DEBUG_ONLY(ft_putstr(" allocs\n\t"));
	FTMALLOC_DEBUG_ONLY(ft_putnbr(g_ftmalloc_state.usage_dealloc));
	FTMALLOC_DEBUG_ONLY(ft_putstr(" deallocs\n\t"));
	FTMALLOC_DEBUG_ONLY(ft_putnbr(g_ftmalloc_state.usage_realloc));
	FTMALLOC_DEBUG_ONLY(ft_putstr(" reallocs("));
	FTMALLOC_DEBUG_ONLY(ft_putnbr(g_ftmalloc_state.total_realloc_hits));
	FTMALLOC_DEBUG_ONLY(ft_putstr(" hits)\n\t"));
	FTMALLOC_DEBUG_ONLY(ft_putnbr(g_ftmalloc_state.total_alloc));
	FTMALLOC_DEBUG_ONLY(ft_putstr(" bytes allocated("));
	FTMALLOC_DEBUG_ONLY(ft_putnbr(g_ftmalloc_state.total_alloc_copied));
	FTMALLOC_DEBUG_ONLY(ft_putstr(" copied)\n\t"));
	FTMALLOC_DEBUG_ONLY(ft_putnbr(g_ftmalloc_state.total_dealloc));
	FTMALLOC_DEBUG_ONLY(ft_putstr(" bytes deallocated\n"));
	FTMALLOC_UNLOCK;
}

void	show_alloc_mem()
{
	ftmalloc_show_mem();
}

void	show_alloc_mem_ex()
{
	ftmalloc_show_mem_ex();
}
