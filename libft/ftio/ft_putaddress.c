/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putaddress.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydzhuryn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/25 16:52:00 by ydzhuryn          #+#    #+#             */
/*   Updated: 2017/10/28 16:43:43 by ydzhuryn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#define HEX "0123456789ABCDEF"

void	ft_putaddress(void *addr)
{
	char	b[sizeof(void*) * 2 + 1];
	int		i;
	size_t	a;

	ft_memset(b, '0', sizeof(b));
	b[sizeof(b) - 1] = 0x0;
	a = *((size_t*)&addr);
	i = sizeof(b) - 1;
	while (a)
	{
		b[--i] = HEX[a % 16];
		a /= 16;
	}
	ft_putstr(b);
}
