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

/*
** General Purpose Allocators
*/
void	*ftmalloc(size_t size);
void    *ftcalloc(size_t num, size_t size);
void	*ftrealloc(void *mem, size_t size);
void	ftfree(void *mem);

/*
** Debug
*/
void	ftmalloc_show_mem();
void	ftmalloc_show_mem_ex();
t_bool  ftmalloc_check_heap_relaxed();
t_bool  ftmalloc_check_heap_fully();

#endif
