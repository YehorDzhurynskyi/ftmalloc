/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftmalloc_internal.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydzhuryn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/15 15:08:24 by ydzhuryn          #+#    #+#             */
/*   Updated: 2018/01/05 17:20:48 by ydzhuryn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FTMALLOC_INTERNAL_H
# define FTMALLOC_INTERNAL_H

#ifndef WIN32
# include "ft.h"
# include <sys/mman.h>
# include <unistd.h>
#else
# include "ftmalloc_windows_adaptor.h"
#endif
# include "ftmalloc.h"
# include "ftmalloc_decl.h"
# include <stddef.h>
# include <errno.h>
# include <stdlib.h>

/*
** INTERNAL GENERAL PURPOSE ALLOCATORS
*/
void			*ftmalloc_internal(size_t size);
void			*ftcalloc_internal(size_t num, size_t size);
void			*ftrealloc_internal(void *mem, size_t size);
void			ftfree_internal(void *mem);

/*
** COMMON
*/
t_bool			ftmalloc_size_request_is_out_of_range(size_t size);
void			ftmalloc_call_prologue(void);

/*
** DEBUG
*/
void			bin_verify(const t_mem_bin *bin);
void			chunk_verify(const t_mem_chunk *chunk);

/*
** SHOW MEM
*/
typedef void	(*t_show_chunk_func)(t_mem_chunk*);
void			show_mem_chunk(t_mem_chunk *chunk);
void			show_mem_chunk_ex(t_mem_chunk *chunk);
void			show_mem_bin(t_mem_bin *bin, t_show_chunk_func show_func);
void			show_mem_internal(t_show_chunk_func show_func);

/*
** MEM
*/
t_bool			mem_inpool(void *mem);
size_t			mem_reserve(t_mem *mem, const size_t size);
size_t			mem_find(t_mem *mem, const size_t size);
size_t			mem_allocate(t_mem *mem, const size_t size);
void			mem_deallocate(t_mem *mem, const size_t size, t_mem_bin *prev);

/*
** BIN
*/
t_mem_bin		*bin_init(t_byte *mem, size_t size);

t_bool			bin_is_empty(const t_mem_bin *bin);

t_byte			*bin_bin2mem(t_mem_bin *bin);

t_mem_bin		*bin_list_of(size_t size);
size_t			bin_alloc_size_of(size_t size);
size_t			bin_max_size_of(size_t size);

t_mem_chunk		*bin_adj_top(const t_mem_bin *bin);
t_mem_chunk     *bin_adj_bottom(const t_mem_bin* bin);

/*
** BUDDY
*/
void			buddy_occupy(t_mem *mem, const size_t size);
void			buddy_deoccupy(t_mem *mem);
t_mem_chunk		*buddy_try_merge_next(t_mem *mem);
t_mem_chunk		*buddy_try_merge_prev(t_mem *mem);

/*
** CHUNK
*/
t_mem_chunk		*chunk_arrange(t_byte *mem, size_t size, t_bool inuse);

t_mem_bin		*chunk_bin_of(const t_mem_chunk *chunk, t_mem_bin **prev_bin);
t_mem_bin		*chunk_bin_of_slow(const t_mem_chunk *chunk);

t_bool			chunk_is_splittable(const size_t oldsize, const size_t newsize);

t_bool			chunk_is_prev_top(const t_mem_chunk *chunk);
t_bool			chunk_is_next_bottom(const t_mem_chunk *chunk);

t_byte			*chunk_chunk2mem(t_mem_chunk *chunk);
t_mem_chunk		*chunk_mem2chunk(t_byte *mem);

void			chunk_prev_in_use_set_true(t_mem_chunk *chunk);
void			chunk_prev_in_use_set_false(t_mem_chunk *chunk);
t_bool			chunk_prev_in_use_get(const t_mem_chunk *chunk);

void			chunk_in_use_set_true(t_mem_chunk *chunk);
void			chunk_in_use_set_false(t_mem_chunk *chunk);
t_bool			chunk_in_use_get(const t_mem_chunk *chunk);

void			chunk_size_set(t_mem_chunk *chunk, size_t size);
size_t			chunk_size_get(const t_mem_chunk *chunk);

t_mem_chunk		*chunk_adj_prev(const t_mem_chunk *chunk);
t_mem_chunk		*chunk_adj_next(const t_mem_chunk *chunk);

t_mem_chunk		*chunk_freed_prev_get(const t_mem_chunk *chunk);
void			chunk_freed_prev_set(t_mem_chunk *chunk, t_mem_chunk *freed);
t_mem_chunk		*chunk_freed_next_get(const t_mem_chunk *chunk);
void			chunk_freed_next_set(t_mem_chunk *chunk, t_mem_chunk *freed);

/*
** REALLOC
*/
t_bool			realloc_try_grow(t_mem *mem, size_t size, size_t osize);
t_bool			realloc_try_shrink(t_mem *mem, size_t size, size_t osize);

#endif
