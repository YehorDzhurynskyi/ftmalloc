/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_mem_chunk.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydzhuryn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/15 15:08:24 by ydzhuryn          #+#    #+#             */
/*   Updated: 2018/01/05 17:20:48 by ydzhuryn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftmalloc_internal.h"

void	show_mem_chunk(t_mem_chunk *chunk)
{
	chunk_verify(chunk);
	ft_putstr("\t\tAddress: ");
	ft_putaddress((void*)chunk);
	ft_putstr("\n\t\tUser address: ");
	ft_putaddress((void*)chunk_chunk2mem(chunk));
	ft_putstr("\n\t\tSize: ");
	ft_putnbr((chunk_in_use_get(chunk) ? chunk_size_get(chunk) : 0)
	+ FTMALLOC_CHUNK_SZ);
	ft_putstr(" Occupied(");
	ft_putnbr((chunk_in_use_get(chunk) ? chunk_size_get(chunk) : 0));
	ft_putstr(" User) / ");
	ft_putnbr(chunk_in_use_get(chunk) ? 0 : chunk_size_get(chunk));
	ft_putstr(" Available\n\t\tFree: ");
	ft_putstr(chunk_in_use_get(chunk) ? "No" : "Yes");
	ft_putstr("\n");
	if (!chunk_in_use_get(chunk))
	{
		ft_putstr("\t\tNext Chunk: ");
		ft_putaddress((void*)chunk_freed_next_get(chunk));
		ft_putstr("\n\t\tPrev Chunk: ");
		ft_putaddress((void*)chunk_freed_prev_get(chunk));
		ft_putstr("\n");
	}
	ft_putstr("\n");
}
