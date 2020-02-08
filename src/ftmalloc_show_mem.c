/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftmalloc_show_mem.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydzhuryn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/15 15:08:24 by ydzhuryn          #+#    #+#             */
/*   Updated: 2018/01/05 17:20:48 by ydzhuryn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftmalloc_internal.h"

static void show_chunk(const t_mem_chunk *chunk)
{
    _chunk_verify(chunk);

    ft_printf("\t\tAddress: %p\n\t\tUser address: %p\n", chunk, _chunk_chunk2mem(chunk));
    ft_printf("\t\tSize: %zu Occupied(%zu User) / %zu Available\n",
              (_chunk_in_use_get(chunk) ? _chunk_size_get(chunk) : 0) + FTMALLOC_MEM_CHUNK_SZ,
              _chunk_in_use_get(chunk) ? _chunk_size_get(chunk) : 0,
              _chunk_in_use_get(chunk) ? 0 : _chunk_size_get(chunk));

    ft_printf("\t\tFree: %s\n", _chunk_in_use_get(chunk) ? "No" : "Yes");

    if (!_chunk_in_use_get(chunk))
    {
        ft_printf("\t\tNext Chunk: %p\n", _chunk_freed_next_get(chunk));
        ft_printf("\t\tPrev Chunk: %p\n", _chunk_freed_prev_get(chunk));
    }

    ft_printf("\n");
}

static void show_chunk_ex(const t_mem_chunk *chunk)
{
    show_chunk(chunk);

    if (!_chunk_in_use_get(chunk))
    {
        return;
    }

    size_t size = _chunk_size_get(chunk);
    t_byte* mem = _chunk_chunk2mem(chunk);
    ft_printf("\t\t0x%p: ", mem);
    unsigned int i = 0;
    do
    {
        t_byte b = mem[i++];
        if (isprint(b))
        {
            ft_printf("%2c", b);
        }
        else
        {
            ft_printf("%02x", b);
        }

        if (i < size)
        {
            if (i % 16 == 0)
            {
                ft_printf("\n");
                ft_printf("\t\t0x%p: ", mem + i);
            }
            else
            {
                ft_printf(" ");
            }
        }
    } while (i < size);

    ft_printf("\n\n\n");
}

static void show_bin(const t_mem_bin *bin)
{
    t_mem_chunk *chunk;

    _bin_verify(bin);
    ft_printf("\t%zu Occupied(%zu User, %zu Bin Header) / %zu Available / %zu Total\n",
              bin->mem_occupied,
              bin->mem_user,
              FTMALLOC_MEM_BIN_SZ,
              bin->mem_allocated - bin->mem_occupied,
              bin->mem_allocated);

    ft_printf("\tHead: %p\n", bin->head);
    ft_printf("\tNext Bin: %p\n", bin->next);
    chunk = _bin_adj(bin);
    while (!_chunk_is_prev_top(chunk))
    {
        show_chunk_ex(chunk);
        chunk = _chunk_adj_prev(chunk);
    }
    show_chunk_ex(chunk);
    ft_printf("\n");
}

void ftmalloc_show_mem()
{
    t_mem_bin *bin;

	ft_printf(" === HEAP USAGE ===\n");

    bin = g_ftmalloc_state.bin_list_small;
    ft_printf("Small Bucket%s\n", bin ? ":" : " (Empty)");
    while (bin)
    {
        show_bin(bin);
        bin = bin->next;
    }
    bin = g_ftmalloc_state.bin_list_medium;
	ft_printf("Medium Bucket%s\n", bin ? ":" : " (Empty)");
    while (bin)
    {
        show_bin(bin);
        bin = bin->next;
    }
    bin = g_ftmalloc_state.bin_list_large;
	ft_printf("Large Bucket%s\n", bin ? ":" : " (Empty)");
    while (bin)
    {
        show_bin(bin);
        bin = bin->next;
    }

// TODO: move to ex version
#ifdef FTMALLOC_DEBUG
	ft_printf("\nTotal Heap Usage:\n\t%i mmap\n\t%i munmap\n\t%i allocs\n\t%i deallocs\n\t%i reallocs(%i hits)\n\t%zu bytes allocated(%zu copied)\n\t%zu bytes deallocated\n",
        g_ftmalloc_state.total_mmap,
        g_ftmalloc_state.total_munmap,
        g_ftmalloc_state.usage_alloc,
		g_ftmalloc_state.usage_dealloc,
        g_ftmalloc_state.usage_realloc,
        g_ftmalloc_state.total_realloc_hits,
        g_ftmalloc_state.total_alloc,
        g_ftmalloc_state.total_alloc_copied,
        g_ftmalloc_state.total_dealloc);
#endif
}

void ftmalloc_show_mem_ex()
{
	// TODO: implement
	ftmalloc_show_mem();
}
