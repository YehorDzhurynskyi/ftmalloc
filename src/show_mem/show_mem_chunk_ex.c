/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_mem_chunk_ex.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydzhuryn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/15 15:08:24 by ydzhuryn          #+#    #+#             */
/*   Updated: 2018/01/05 17:20:48 by ydzhuryn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftmalloc_internal.h"

static void	print_hex_dump(t_byte *raw, size_t size)
{
	t_byte	b;
	int		i;

	ft_printf("\t\t0x%p: ", raw);
	i = 0;
	while (i < size)
	{
		b = raw[i++];
		if (ft_isprint(b))
			ft_printf("%2c", b);
		else
			ft_printf("%02x", b);
		if (i < size)
		{
			if (i % 16 == 0)
				ft_printf("\n\t\t0x%p: ", raw + i);
			else
				ft_printf(" ");
		}
	}
}

void		show_mem_chunk_ex(t_mem_chunk *chunk)
{
	show_mem_chunk(chunk);
	if (!chunk_in_use_get(chunk))
	{
		return ;
	}
	print_hex_dump(chunk_chunk2mem(chunk), chunk_size_get(chunk));
	ft_printf("\n\n\n");
}
