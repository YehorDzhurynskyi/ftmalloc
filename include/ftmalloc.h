/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftmalloc.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydzhuryn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/15 15:08:24 by ydzhuryn          #+#    #+#             */
/*   Updated: 2018/01/05 17:20:48 by ydzhuryn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FTMALLOC_H
# define FTMALLOC_H

# include <stddef.h>

# define FTMALLOC_ENV_SCRIBBLE				"MallocScribble"
# define FTMALLOC_ENV_CHECK_HEAP_RELAXED	"MallocCheckHeapRelaxed"
# define FTMALLOC_ENV_CHECK_HEAP_FULLY		"MallocCheckHeapFully"

/*
** General Purpose Allocators
*/
void	*ftmalloc(size_t size);
void	*ftcalloc(size_t num, size_t size);
void	*ftrealloc(void *mem, size_t size);
void	ftfree(void *mem);

void	show_alloc_mem(void);
void	show_alloc_mem_ex(void);

/*
** Debug
*/
void	ftmalloc_show_mem(void);
void	ftmalloc_show_mem_ex(void);
int		ftmalloc_check_heap_relaxed(void);
int		ftmalloc_check_heap_fully(void);

#endif
