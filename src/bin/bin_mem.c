/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bin_mem.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydzhuryn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/15 15:08:24 by ydzhuryn          #+#    #+#             */
/*   Updated: 2018/01/05 17:20:48 by ydzhuryn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftmalloc_internal.h"

t_byte	*bin_bin2mem(t_mem_bin *bin)
{
	bin_verify(bin);
	return ((t_byte*)bin->head - FTMALLOC_MEM_CHUNK_SZ);
}

t_bool	bin_is_empty(const t_mem_bin *bin)
{
	return (bin->mem_user == 0);
}
