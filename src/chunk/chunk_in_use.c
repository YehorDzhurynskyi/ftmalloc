/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk_in_use.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydzhuryn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/15 15:08:24 by ydzhuryn          #+#    #+#             */
/*   Updated: 2018/01/05 17:20:48 by ydzhuryn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftmalloc_internal.h"

void	chunk_in_use_set_true(t_mem_chunk *chunk)
{
	FTMALLOC_ASSERT(chunk != NULL);
	chunk_prev_in_use_set_true(chunk_adj_next(chunk));
}

void	chunk_in_use_set_false(t_mem_chunk *chunk)
{
	FTMALLOC_ASSERT(chunk != NULL);
	chunk_prev_in_use_set_false(chunk_adj_next(chunk));
}

t_bool	chunk_in_use_get(const t_mem_chunk *chunk)
{
	FTMALLOC_ASSERT(chunk != NULL);
	return (chunk_prev_in_use_get(chunk_adj_next(chunk)));
}
