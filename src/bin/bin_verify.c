/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftmalloc_bin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydzhuryn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/15 15:08:24 by ydzhuryn          #+#    #+#             */
/*   Updated: 2018/01/05 17:20:48 by ydzhuryn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftmalloc_internal.h"

void	bin_verify(const t_mem_bin *bin)
{
	FTMALLOC_ASSERT(bin);
	FTMALLOC_ASSERT(FTMALLOC_MEM_ALIGNED_OK(bin->mem_allocated));
	FTMALLOC_ASSERT(FTMALLOC_MEM_ALIGNED_OK(bin->mem_occupied));
	FTMALLOC_ASSERT(FTMALLOC_MEM_ALIGNED_OK(bin->mem_user));
	FTMALLOC_ASSERT(bin->mem_occupied <= bin->mem_allocated);
	FTMALLOC_ASSERT(bin->mem_user < bin->mem_occupied);
	FTMALLOC_ASSERT(FTMALLOC_MEM_ALIGNED_OK(bin->next));
	if (bin->head != NULL)
	{
		FTMALLOC_ASSERT(FTMALLOC_MEM_ALIGNED_OK(bin->head));
		FTMALLOC_ASSERT(!chunk_in_use_get(bin->head));
		FTMALLOC_ASSERT(chunk_freed_prev_get(bin->head) == NULL);
		chunk_verify(bin->head);
	}
}
