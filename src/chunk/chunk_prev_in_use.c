/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk_prev_in_use.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydzhuryn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/15 15:08:24 by ydzhuryn          #+#    #+#             */
/*   Updated: 2018/01/05 17:20:48 by ydzhuryn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftmalloc_internal.h"

void	chunk_prev_in_use_set_true(t_mem_chunk *chunk)
{
	FTMALLOC_ASSERT(chunk != NULL);
	chunk->size |= FTMALLOC_PREV_IN_USE_BIT;
}

void	chunk_prev_in_use_set_false(t_mem_chunk *chunk)
{
	FTMALLOC_ASSERT(chunk != NULL);
	chunk->size &= ~FTMALLOC_PREV_IN_USE_BIT;
}

t_bool	chunk_prev_in_use_get(const t_mem_chunk *chunk)
{
	FTMALLOC_ASSERT(chunk != NULL);
	return (chunk->size & FTMALLOC_PREV_IN_USE_BIT);
}
