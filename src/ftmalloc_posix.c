/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftmalloc_posix.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydzhuryn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/15 15:08:24 by ydzhuryn          #+#    #+#             */
/*   Updated: 2018/01/05 17:20:48 by ydzhuryn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftmalloc_internal.h"

#ifdef FTMALLOC_POSIX_API

void	*malloc(size_t size)
{
	return (ftmalloc(size));
}

void	*calloc(size_t num, size_t size)
{
	return (ftcalloc(num, size));
}

void	*realloc(void *mem, size_t size)
{
	return (ftrealloc(mem, size));
}

void	free(void *mem)
{
	ftfree(mem);
}

#endif
