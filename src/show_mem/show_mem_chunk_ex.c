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

#define HEX "0123456789ABCDEF"

static void	print_printable(t_byte b)
{
	FTMALLOC_ASSERT(ft_isprint(b));
	ft_putchar(' ');
	ft_putchar(b);
}

static void	print_unprintable(t_byte b)
{
	FTMALLOC_ASSERT(!ft_isprint(b));
	ft_putchar(HEX[b / 0x10]);
	ft_putchar(HEX[b % 0x10]);
}

static void	print_addrline(t_byte *raw)
{
	ft_putstr("\t\t0x");
	ft_putaddress((void*)raw);
	ft_putstr(": ");
}

static void	print_hex_dump(t_byte *raw, size_t size)
{
	t_byte	b;
	size_t	i;

	print_addrline(raw);
	i = 0;
	while (i < size)
	{
		b = raw[i++];
		if (ft_isprint(b))
			print_printable(b);
		else
			print_unprintable(b);
		if (i < size)
		{
			if (i % 16 == 0)
			{
				ft_putstr("\n");
				print_addrline(raw + i);
			}
			else
				ft_putstr(" ");
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
	ft_putstr("\n\n\n");
}
